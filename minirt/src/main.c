#include "minirt.h"

int handle_key(int keycode, t_scene *scene)
{
    (void)scene;
    if (keycode == 65307) /* ESC key */
        exit(0);
    return 0;
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
