#include "minirt.h"
#include "camera.h"
#include "math_utils.h"
#include "mlx_init.h"

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

int hit_plane(t_vec3 point_on_plane, t_vec3 normal, t_ray ray, double *out_t)
{
    double denom = vec3_dot(normal, ray.direction);
    if (fabs(denom) < 1e-6)
        return 0;

    double t = vec3_dot(vec3_sub(point_on_plane, ray.origin), normal) / denom;
    if (t < 0)
        return 0;

    *out_t = t;
    return 1;
}

int intersect_ray_sphere(t_ray ray , t_sphere sphere, double *out_t, t_vec3 *out_hit_point)
{
    t_vec3 oc = vec3_sub(ray.origin, sphere.center);
    double b = vec3_dot(oc, ray.direction);
    double c = vec3_dot(oc, oc) - sphere.radius * sphere.radius;
    double discriminant = b * b - c;

    if (discriminant < 0.0)
        return 0;

    double t_hit = -b - sqrt(discriminant);
    if (t_hit < 0.0)
        t_hit = -b + sqrt(discriminant);
    if (t_hit < 0.0)
        return 0;

    *out_t = t_hit;
    *out_hit_point = vec3_add(ray.origin, vec3_mult(ray.direction, t_hit));
    return 1;
}

t_vec3 compute_lighting(t_point3 hit_point, t_vec3 normal, t_vec3 object_color, t_light light)
{
    // Ambient
    double ambient_strength = 0.1;
    t_vec3 ambient = vec3_mult(object_color, ambient_strength);

    // Diffuse
    t_vec3 light_dir = vec3_normalize(vec3_sub(light.position, hit_point));
    double diff = fmax(vec3_dot(normal, light_dir), 0.0);
    t_vec3 diffuse = vec3_mult(vec3_mul_vec(light.intensity, object_color), diff);

    return vec3_add(ambient, diffuse);
}

t_vec3 background_color(t_ray ray)
{
    t_vec3 dir = vec3_normalize(ray.direction);
    double t = 0.5 * (dir.y + 1.0);
    t_vec3 white = {1.0, 1.0, 1.0};
    t_vec3 blue = {0.5, 0.7, 1.0};
    return vec3_add(vec3_mult(white, 1.0 - t), vec3_mult(blue, t));
}

void render(t_scene *scene)
{
    t_image *img = &scene->image;

    t_light light = {
        .position = (t_vec3){5, 5, 0},
        .intensity = (t_vec3){1.0, 1.0, 1.0}
    };

    t_sphere s = {
        .center = (t_point3){0, 0, 5},
        .radius = 1.0
    };

    t_sphere s1 = {
        .center = (t_point3){0, 0, 10},
        .radius = 5.0
    };

    t_plane plane = {
        .normal = (t_vec3){0, 0, 1},
        .point = (t_vec3){0, 0, 1}
    };

    t_vec3 red = {1.0, 0.0, 0.0};
    t_vec3 green = {0.0, 1.0, 0.0};
    t_vec3 gray = {0.3, 0.3, 0.3};

    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            t_ray ray = {
                .origin = scene->camera.origin,
                .direction = get_ray_direction(&scene->camera, i, j, HEIGHT, WIDTH)
            };

            double t;
            t_vec3 hit_point;
            t_vec3 color = background_color(ray);

            if (intersect_ray_sphere(ray, s, &t, &hit_point)) {
                t_vec3 normal = vec3_normalize(vec3_sub(hit_point, s.center));
                color = compute_lighting(hit_point, normal, red, light);
            }
            else if (intersect_ray_sphere(ray, s1, &t, &hit_point)) {
                t_vec3 normal = vec3_normalize(vec3_sub(hit_point, s1.center));
                color = compute_lighting(hit_point, normal, green, light);
            }
            else if (hit_plane(plane.point, plane.normal, ray, &t)) {
                hit_point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
                t_vec3 normal = plane.normal;
                color = compute_lighting(hit_point, normal, gray, light);
            }

            int rgb = rgb_to_int(color.x, color.y, color.z);
            my_mlx_pixel_put(img, i, j, rgb);
        }
    }

    mlx_put_image_to_window(scene->vars.mlx, scene->vars.win, img->img, 0, 0);
}

int handle_key(int keycode, t_scene *scene)
{
    printf("key pressed : %d\n", keycode);

    double step = 0.5;
    if (keycode == 65307) // ESC
        exit(0);
    if (keycode == 119) // W
        scene->camera.origin = vec3_add(scene->camera.origin, vec3_mult(scene->camera.forward, step));
    if (keycode == 115) // S
        scene->camera.origin = vec3_sub(scene->camera.origin, vec3_mult(scene->camera.forward, step));
    if (keycode == 97)  // A
        scene->camera.origin = vec3_sub(scene->camera.origin, vec3_mult(scene->camera.right, step));
    if (keycode == 100) // D
        scene->camera.origin = vec3_add(scene->camera.origin, vec3_mult(scene->camera.right, step));

    compute_camera_basis(&scene->camera);
    setup_viewport(&scene->camera);
    render(scene);
    return 0;
}

t_scene scene_init(void)
{
    t_scene scene;

    scene.vars.mlx = mlx_init();
    scene.vars.win = mlx_new_window(scene.vars.mlx, WIDTH, HEIGHT, "MiniRT Room");
    scene.image = init_image(scene.vars.mlx, WIDTH, HEIGHT);
    scene.camera = init_camera(
        (t_vec3){0, 0, 0},
        (t_vec3){0, 0, 1},
        90.0,
        (double)WIDTH / HEIGHT
    );

    return scene;
}

int main(void)
{
    t_scene scene = scene_init();

    compute_camera_basis(&scene.camera);
    setup_viewport(&scene.camera);
    render(&scene);

    mlx_hook(scene.vars.win, 2, 1L << 0, handle_key, &scene);
    mlx_loop(scene.vars.mlx);
    return 0;
}
