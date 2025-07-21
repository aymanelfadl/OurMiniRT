#include "minirt.h"

int mouse_move_hook(int keycode ,int x, int y, void *param)
{
    t_scene *scene = (t_scene *)param;

    printf("Mouse: keycode=%d, x=%d, y=%d\n", keycode, x, y);

    // Build a ray from camera through pixel (x, y)
    t_vec3 dir = get_ray_direction(&scene->camera, x, y, WIDTH, HEIGHT);
    t_ray  ray = (t_ray){scene->camera.origin, dir};

    // Trace the ray and update the pixel under the mouse
    trace_mouse_ray(scene, &ray, x, y);

    // Update the window with the modified pixel
    mlx_put_image_to_window(scene->vars.mlx, scene->vars.win, scene->image.img, 0, 0);

    return (0);
}

void trace_mouse_ray(t_scene *scene, t_ray *ray, int x, int y)
{
    t_hit a9rab_hit = {.t = INFINITY};
    t_list *node = scene->meshes;

    while (node)
    {
        t_object *object = node->content;
        t_hit current_hit;
        int is_hit = 0;

        if (object->type == SPHERE)
            is_hit = ray_sphere(ray, &object->sphere, &current_hit);
        else if (object->type == PLANE)
            is_hit = ray_plane(ray, &object->plane, &current_hit);
        else if (object->type == CYLINDER)
            is_hit = ray_cylinder(ray, &object->cylinder, &current_hit);

        if (is_hit && current_hit.t < a9rab_hit.t)
            a9rab_hit = current_hit;

        node = node->next;
    }

    if (a9rab_hit.t == INFINITY)
        return;

    // Example: override hit color to red to show it's selected
    a9rab_hit.color = (t_vec3){255, 0, 0};

    t_vec3 c = compute_lighting(a9rab_hit, scene);
    my_mlx_pixel_put(&scene->image, x, y, rgb_to_int(c.x, c.y, c.z));
}

t_vec3 trace_ray(t_scene *scene, t_ray *ray)
{
    t_hit a9rab_hit = {.t = INFINITY};
    t_list *node = scene->meshes;

    while (node)
    {
        t_object *object = node->content;
        t_hit current_hit;
        int is_hit = 0;

        if (object->type == SPHERE)
            is_hit = ray_sphere(ray, &object->sphere, &current_hit);
        else if (object->type == PLANE)
            is_hit = ray_plane(ray, &object->plane, &current_hit);
        else if (object->type == CYLINDER)
            is_hit = ray_cylinder(ray, &object->cylinder, &current_hit);
        if (is_hit && current_hit.t < a9rab_hit.t)
            a9rab_hit = current_hit;

        node = node->next;
    }

    if (a9rab_hit.t == INFINITY)
        return (t_vec3){50, 50, 80};

    return compute_lighting(a9rab_hit, scene);
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

            /* 4. Write pixel into image buffer. */
            my_mlx_pixel_put(&scene->image, i, j, rgb_to_int(c.x, c.y, c.z));
        }
    }
    mlx_put_image_to_window(scene->vars.mlx, scene->vars.win, img->img, 0, 0);
}