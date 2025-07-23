#include "minirt.h"

/* Move any object by a direction vector */
static void move_object(t_object *obj, t_vec3 v)
{
    if (obj->type == SPHERE)
        obj->sphere.center = vec3_add(obj->sphere.center, v);
    else if (obj->type == PLANE)
        obj->plane.point = vec3_add(obj->plane.point, v);
    else if (obj->type == CYLINDER)
        obj->cylinder.center = vec3_add(obj->cylinder.center, v);
}

/* Move the camera position */
static void move_camera(t_camera *cam, t_vec3 v)
{
    cam->origin = vec3_add(cam->origin, v);
}

/* Rotate a vector around the Y axis */
t_vec3 rotate_y(t_vec3 v, float angle_deg)
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

/* Rotate selected object around its Y axis */
static void rotate_object(t_object *obj, float angle_deg)
{
    if (obj->type == CYLINDER)
        obj->cylinder.axis = vec3_normalize(rotate_y(obj->cylinder.axis, angle_deg));
    else if (obj->type == PLANE)
        obj->plane.normal = vec3_normalize(rotate_y(obj->plane.normal, angle_deg));
}

/* Handle key input */
int key_hook(int keycode, void *scene)
{
    t_scene *s = (t_scene *)scene;
    t_vec3 obj_dir = {0};     // Direction to move selected object
    t_vec3 cam_dir = {0};     // Direction to move camera
    float  speed   = 0.5f;    // Movement speed

    printf("Key pressed: %d\n", keycode);

    // Object movement keys (WASD + arrows)
    if (keycode == 119)        obj_dir.z =  speed;   // W
    else if (keycode == 115)   obj_dir.z = -speed;   // S
    else if (keycode == 97)    obj_dir.x = -speed;   // A
    else if (keycode == 100)   obj_dir.x =  speed;   // D
    else if (keycode == 65362) obj_dir.y =  speed;   // ↑
    else if (keycode == 65364) obj_dir.y = -speed;   // ↓

    // Object rotation (Q, E)
    else if (keycode == 113 && s->selected_mesh)      // Q
        rotate_object(s->selected_mesh, -5.0f);
    else if (keycode == 101 && s->selected_mesh)      // E
        rotate_object(s->selected_mesh, 5.0f);

    // Camera movement (IJKLUO)
    else if (keycode == 105)   cam_dir.z =  speed;    // I
    else if (keycode == 107)   cam_dir.z = -speed;    // K
    else if (keycode == 106)   cam_dir.x = -speed;    // J
    else if (keycode == 108)   cam_dir.x =  speed;    // L
    else if (keycode == 117)   cam_dir.y =  speed;    // U
    else if (keycode == 111)   cam_dir.y = -speed;    // O

    // Exit (ESC)
    else if (keycode == 65307)
        exit(0);

    // If object selected and movement requested
    if (s->selected_mesh && (obj_dir.x || obj_dir.y || obj_dir.z))
    {
        t_vec3 move = vec3_add(
                         vec3_add(vec3_mult(s->camera.right,   obj_dir.x),
                                  vec3_mult(s->camera.up,      obj_dir.y)),
                                  vec3_mult(s->camera.forward, obj_dir.z));
        move_object(s->selected_mesh, move);
    }

    // Move camera
    if (cam_dir.x || cam_dir.y || cam_dir.z)
    {
        t_vec3 move = vec3_add(
                         vec3_add(vec3_mult(s->camera.right,   cam_dir.x),
                                  vec3_mult(s->camera.up,      cam_dir.y)),
                                  vec3_mult(s->camera.forward, cam_dir.z));
        move_camera(&s->camera, move);
    }

    render(s);
    return 0;
}
