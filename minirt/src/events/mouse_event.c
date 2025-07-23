#include "minirt.h"

int mouse_move_hook(int keycode, int x, int y, void *param)
{
    t_scene *scene = (t_scene *)param;

    printf("Mouse: keycode=%d, x=%d, y=%d\n", keycode, x, y);

    // Build ray from camera
    t_vec3 dir = get_ray_direction(&scene->camera, x, y, WIDTH, HEIGHT);
    t_ray ray = (t_ray){scene->camera.origin, dir};

    trace_mouse_ray(scene, &ray);
    render(scene);

    return 0;
}

void trace_mouse_ray(t_scene *scene, t_ray *ray)
{
    t_hit h = hit_scene(scene->meshes, ray);

    if (h.t != INFINITY)
        scene->selected_mesh = h.object;
    else
        scene->selected_mesh = NULL;
}