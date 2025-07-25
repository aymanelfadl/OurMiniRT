#include "minirt.h"

static t_vec3 rotate_y(t_vec3 v, float angle_deg)
{
    float rad = angle_deg * M_PI / 180.0f;
    float c = cosf(rad);
    float s = sinf(rad);

    t_vec3 result;
    result.x = v.x * c + v.z * s;
    result.y = v.y;
    result.z = -v.x * s + v.z * c;
    return result;
}

static t_vec3 rotate_x(t_vec3 v, float angle_deg)
{
    float rad = angle_deg * M_PI / 180.0f;
    float c = cosf(rad);
    float s = sinf(rad);

    t_vec3 result;
    result.x = v.x;
    result.y = v.y * c - v.z * s;
    result.z = v.y * s + v.z * c;
    return result;
}

static void rotate_object(t_object *obj, float angle_deg, int rotate_axis)
{
    if (obj->type == CYLINDER)
    {
        if (rotate_axis == 1)
            obj->cylinder.axis = vec3_normalize(rotate_y(obj->cylinder.axis, angle_deg));
        else if (rotate_axis == 2)
            obj->cylinder.axis = vec3_normalize(rotate_x(obj->cylinder.axis, angle_deg));
    }
    else if (obj->type == PLANE)
    {
        if (rotate_axis == 1)
            obj->plane.normal = vec3_normalize(rotate_y(obj->plane.normal, angle_deg));
        else if (rotate_axis == 2)
            obj->plane.normal = vec3_normalize(rotate_x(obj->plane.normal, angle_deg));
    }
}

void rotate_objects(t_scene *s, int keycode)
{
    if (keycode == KEY_Q)
        rotate_object(s->selected_mesh, -5.0f, 1);
    else if (keycode == KEY_E)
        rotate_object(s->selected_mesh, 5.0f, 1);
    else if (keycode == 120)
        rotate_object(s->selected_mesh, -5.0f, 2);
    else if (keycode == 99)
        rotate_object(s->selected_mesh, 5.0f, 2);
}

void move_object(t_scene *s, int keycode)
{
    t_vec3 obj_dir = {0};
    t_vec3 move;

    if (keycode == KEY_W)         
        obj_dir.z =  SPEED;
    else if (keycode == KEY_S)
        obj_dir.z = -SPEED;
    else if (keycode == KEY_A)
        obj_dir.x = -SPEED;
    else if (keycode == KEY_D)    
        obj_dir.x =  SPEED;
    else if (keycode == KEY_PLUS)
        obj_dir.y =  SPEED;
    else if (keycode == KEY_MINUS) 
        obj_dir.y = -SPEED;

    move = vec3_add(vec3_add(vec3_mult(s->camera.right,   obj_dir.x),
                                    vec3_mult(s->camera.up,      obj_dir.y)),
                                    vec3_mult(s->camera.forward, obj_dir.z));
    if (s->selected_mesh->type == SPHERE)
        s->selected_mesh->sphere.center = vec3_add(s->selected_mesh->sphere.center, move);
    else if (s->selected_mesh->type == PLANE)
        s->selected_mesh->plane.point = vec3_add(s->selected_mesh->plane.point, move);
    else if (s->selected_mesh->type == CYLINDER)
        s->selected_mesh->cylinder.center = vec3_add(s->selected_mesh->cylinder.center, move);
}

void scale_object(t_scene *s, int keycode)
{
    float scale = 1.0f;

    if (keycode == KEY_F)
        scale = 0.9f;
    else if (keycode == KEY_R)
        scale = 1.2f;

    if (s->selected_mesh->type == SPHERE)
    {
        s->selected_mesh->sphere.radius *= scale;
        s->selected_mesh->sphere.radius = fmaxf(s->selected_mesh->sphere.radius, 0.01f);
    }
    /* add cylinder here if you want to scale radius+height */
}