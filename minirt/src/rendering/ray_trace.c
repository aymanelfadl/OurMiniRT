#include "minirt.h"

t_hit  hit_scene(t_list *meshes, t_ray *ray)
{
    t_hit  closest = (t_hit){ .t = INFINITY };
    t_list *node   = meshes;

    while (node)
    {
        t_object *obj = node->content;
        t_hit     h;
        int       is_hit = 0;

        if      (obj->type == SPHERE)
            is_hit = ray_sphere(ray, &obj->sphere,   &h);
        else if (obj->type == PLANE)
            is_hit = ray_plane    (ray, &obj->plane,    &h);
        else if (obj->type == CYLINDER)
            is_hit = ray_cylinder (ray, &obj->cylinder, &h);
            
        if (is_hit && h.t < closest.t)
        {
            h.object = obj;
            closest  = h;
        }
        node = node->next;
    }
    return (closest);
}

static t_vec3 pixel_color(t_scene *s, int x, int y)
{
    t_ray  ray = { s->camera.origin, get_ray_direction(&s->camera, x, y, WIDTH, HEIGHT) };
    t_hit  hit = hit_scene(s->meshes, &ray);

    if (hit.t == INFINITY)
        return (t_vec3){0, 0, 0};

    t_vec3 c = compute_lighting(hit, s);

    if (s->selected_mesh && hit.object == s->selected_mesh)
    {
        c.x = fmin(c.x + 40, 255);
        c.y = fmin(c.y + 40, 255);
        c.z = fmin(c.z + 40, 255);
    }
    return c;
}

void render(t_scene *scene)
{
    t_image *img = &scene->image;
    int      y;
    int      x;
    
    y = 0; 
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            t_vec3 col = pixel_color(scene, x, y);
            my_mlx_pixel_put(img, x, y, rgb_to_int(col.x, col.y, col.z));
            ++x;
        }
        ++y;
    }
    mlx_put_image_to_window(scene->vars.mlx,
                            scene->vars.win,
                            img->img, 0, 0);
}


