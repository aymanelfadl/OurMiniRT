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

        if (is_hit)
        {
            current_hit.object = object;
            if (current_hit.t < a9rab_hit.t)
            {
                a9rab_hit = current_hit;
                scene->selected_mesh = object;
            }
        }
        node = node->next;
    }
}

t_hit get_a9rabe_hit(t_list *meshes_head, t_ray *ray)
{
    t_list *node = meshes_head;
    t_hit a9rab_hit = {.t = INFINITY};

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

        if (is_hit)
        {
            current_hit.object = object;
            if (current_hit.t < a9rab_hit.t)
                a9rab_hit = current_hit;
        }

        node = node->next;
    }
    return a9rab_hit;
}

t_vec3 trace_ray(t_scene *scene, t_ray *ray)
{
    
    t_hit a9rab_hit;
    a9rab_hit = get_a9rabe_hit(scene->meshes, ray);

    if (a9rab_hit.t == INFINITY)
        return (t_vec3){0, 0, 0};

    return compute_lighting(a9rab_hit, scene);
}

void render(t_scene *scene)
{
    t_image *img = &scene->image;

    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            // Shoot ray
            t_vec3 dir = get_ray_direction(&scene->camera, x, y, WIDTH, HEIGHT);
            t_ray ray = { scene->camera.origin, dir };
            t_hit hit = get_a9rabe_hit(scene->meshes, &ray);

            if (hit.t == INFINITY)
            {
                my_mlx_pixel_put(img, x, y, 0x000000);
                continue;
            }

            // Normal color from lighting
            t_vec3 c = compute_lighting(hit, scene);

            // If hovered (selected object), brighten a bit
            if (scene->selected_mesh && hit.object == scene->selected_mesh)
            {
                c.x = fmin(c.x + 40, 255);
                c.y = fmin(c.y + 40, 255);
                c.z = fmin(c.z + 40, 255);
            }

            my_mlx_pixel_put(img, x, y, rgb_to_int(c.x, c.y, c.z));
        }
    }

    mlx_put_image_to_window(scene->vars.mlx, scene->vars.win, img->img, 0, 0);
}


