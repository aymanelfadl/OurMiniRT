#include "minirt.h"
#include "camera.h"
#include "math_utils.h"
#include "mlx_init.h"
#include "parser.h"

int rgb_to_int(double r, double g, double b)
{
    int ir = (int)(255.999 * fmin(r, 1.0));
    int ig = (int)(255.999 * fmin(g, 1.0));
    int ib = (int)(255.999 * fmin(b, 1.0));
    return (ir << 16) | (ig << 8) | ib;
}

void my_mlx_pixel_put(t_image *img, int x, int y, int color)
{
    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return;
    char *pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
    *(unsigned int *)pixel = color;
}


int handle_key(int keycode, t_scene *scene)
{
    (void)scene;
    if (keycode == 65307) /* ESC key */
        exit(0);
    return 0;
}

t_vec3 compute_lighting(t_point3 hit_point, t_vec3 normal, t_vec3 object_color, t_light light)
{
    // Ambient
    double ambient_strength = 0.1;
    t_vec3 ambient = vec3_mult(object_color, ambient_strength);

    // Diffuse
    t_vec3 light_dir = vec3_normalize(vec3_sub(light.position, hit_point));
    double diff = fmax(vec3_dot(normal, light_dir), 0.0);
    t_vec3 diffuse = vec3_mult(vec3_mul_vec(light.color, object_color), diff);

    return vec3_add(ambient, diffuse);
}

int ray_sphere(t_ray *r, t_sphere *s, t_hit *h)
{
    t_vec3 oc = vec3_sub(r->origin, s->center);
    
    float a = vec3_dot(r->direction, r->direction);
    float b = 2.f * vec3_dot(oc, r->direction);
    float c = vec3_dot(oc, oc) - s->radius * s->radius;

    float disc = b * b - 4.f * a * c;
    if (disc < 0.f)
        return 0;
    float t = (-b - sqrtf(disc)) / (2.f * a);
    
    if (t < 0.f)
        t = (-b + sqrtf(disc)) / (2.f * a);
    
    if (t < 0.f)
        return 0;

    h->t = t;
    h->p = vec3_add(r->origin, vec3_mult(r->direction, t));
    h->n = vec3_normalize(vec3_sub(h->p, s->center));
    h->color = s->color;

    return 1;
}

int ray_plane(const t_ray *r, const t_plane *pl, t_hit *h)
{
    float denom = vec3_dot(pl->normal, r->direction);

    if (fabsf(denom) < 1e-6f)
        return 0;
    t_vec3 diff = vec3_sub(pl->point, r->origin);

    float t = vec3_dot(diff, pl->normal) / denom;
    if (t < 0.f)
        return 0;
    
    h->t = t;
    h->p = vec3_add(r->origin, vec3_mult(r->direction, t));
    h->n = (denom > 0) ? vec3_mult(pl->normal, -1.f) : pl->normal;
    h->color = pl->color;
    
    return 1;
}

t_vec3 trace_ray(t_scene *s, t_ray *r)
{
    t_hit   best = {.t = INFINITY};
    t_list *lst = s->meshes;
    while (lst)
    {
        t_object *obj = (t_object *)lst->content;
        t_hit tmp;
        int hit = 0;
        switch (obj->type)
        {
            case SPHERE:
            {
                hit = ray_sphere(r, &obj->sphere, &tmp);
                break;
            }
            case PLANE:
            {
                hit = ray_plane(r, &obj->plane,  &tmp);
                break;
            }
            case CYLINDER: break;
        }
        if (hit && tmp.t < best.t)
            best = tmp;
        lst = lst->next;
    }
    if (best.t == INFINITY)
        return (t_vec3){50,50,80};

    t_vec3 L = vec3_normalize(vec3_sub(s->light.position, best.p));
    float  Ld = fmaxf(0.f, vec3_dot(best.n, L)) * s->light.brightness;
    t_vec3 col = vec3_mult(best.color, Ld);

    return col;
}

void render(t_scene *scene)
{
    t_image *img = &scene->image;

    for (int j = 0; j < HEIGHT; j++)
    {
        for (int i = 0; i < WIDTH; i++)
        {
            /* 1. Build a ray whose origin is the camera origin. */
            t_vec3 dir = get_ray_direction(&scene->camera, i, j, WIDTH, HEIGHT);
            t_ray  ray = (t_ray){scene->camera.origin, dir};

            /* 2. Trace the ray. */
            t_vec3 c = trace_ray(scene, &ray);

            /* 3. Clamp colour and pack into 0x00RRGGBB. */
            int r = (int)fminf(255, fmaxf(0, c.x));
            int g = (int)fminf(255, fmaxf(0, c.y));
            int b = (int)fminf(255, fmaxf(0, c.z));
            int rgb = (r << 16) | (g << 8) | b;

            /* 4. Write pixel into image buffer. */
            my_mlx_pixel_put(&scene->image, i, j, rgb);
        }
    }
    mlx_put_image_to_window(scene->vars.mlx, scene->vars.win, img->img, 0, 0);
}

static void print_vec3(const char *label, t_vec3 v)
{
    printf("%-12s : [% .3f, % .3f, % .3f]\n", label, v.x, v.y, v.z);
}

void print_scene(const t_scene *s)
{
    if (!s)
    {
        printf("Scene is NULL\n");
        return;
    }

    printf("==========  SCENE  ==========\n");

    /* Ambient */
    printf("Ambient      : ratio = %.2f\n", s->ambient.ratio);
    print_vec3("  color   ", s->ambient.color);

    /* Camera */
    printf("Camera       : fov = %.1f°\n", s->camera.fov_deg);
    print_vec3("  origin  ", s->camera.origin);
    print_vec3("  target  ", s->camera.target);

    /* Light */
    printf("Light        : brightness = %.2f\n", s->light.brightness);
    print_vec3("  pos     ", s->light.position);
    print_vec3("  colour  ", s->light.color);

    printf("=============================\n");
}

void print_meshes(const t_list *meshes)
{
    const t_list *node = meshes;
    int           i    = 0;

    printf("\n----------  MESHES  ----------\n");
    while (node)
    {
        t_object *obj = (t_object *)node->content;
        switch (obj->type)
        {
            case SPHERE:
                printf("[%d] SPHERE\n", i++);
                print_vec3("  center", obj->sphere.center);
                printf("  radius : %.3f\n", obj->sphere.radius);
                print_vec3("  color ", obj->sphere.color);
                break;
            case PLANE:
                printf("[%d] PLANE\n", i++);
                print_vec3("  point ", obj->plane.point);
                print_vec3("  normal", obj->plane.normal);
                print_vec3("  color ", obj->plane.color);
                break;
            case CYLINDER:
                printf("[%d] CYLINDER\n", i++);
                print_vec3("  center", obj->cylinder.center);
                print_vec3("  axis  ", obj->cylinder.axis);
                printf("  diam  : %.3f\n", obj->cylinder.diameter);
                printf("  height: %.3f\n", obj->cylinder.height);
                print_vec3("  color ", obj->cylinder.color);
                break;
            default:
                printf("[%d] UNKNOWN TYPE\n", i++);
                break;
        }
        node = node->next;
    }
    printf("------------------------------\n");
}

int main(int argc, char *argv[])
{
    (void) argc;
    t_scene *scene = scene_init(argv[1]);

    print_scene(scene);
    print_meshes(scene->meshes);
    
    compute_camera_basis(&scene->camera);
    setup_viewport(&scene->camera);
    render(scene);
    
    mlx_hook(scene->vars.win, 2, 1L << 0, handle_key, scene);
    mlx_loop(scene->vars.mlx);
    free(scene);
    return 0;
}
