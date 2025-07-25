#include "minirt.h"



/* Move selected object */
static void move_object(t_scene *s, int keycode)
{
    t_vec3 obj_dir = {0};

    if (keycode == KEY_W)         obj_dir.z =  SPEED;
    else if (keycode == KEY_S)    obj_dir.z = -SPEED;
    else if (keycode == KEY_A)    obj_dir.x = -SPEED;
    else if (keycode == KEY_D)    obj_dir.x =  SPEED;
    else if (keycode == KEY_PLUS) obj_dir.y =  SPEED;
    else if (keycode == KEY_MINUS) obj_dir.y = -SPEED;

    t_vec3 move = vec3_add(vec3_add(vec3_mult(s->camera.right,   obj_dir.x),
                                    vec3_mult(s->camera.up,      obj_dir.y)),
                                    vec3_mult(s->camera.forward, obj_dir.z));

    if (s->selected_mesh->type == SPHERE)
        s->selected_mesh->sphere.center = vec3_add(s->selected_mesh->sphere.center, move);
    else if (s->selected_mesh->type == PLANE)
        s->selected_mesh->plane.point = vec3_add(s->selected_mesh->plane.point, move);
    else if (s->selected_mesh->type == CYLINDER)
        s->selected_mesh->cylinder.center = vec3_add(s->selected_mesh->cylinder.center, move);
}

/* Move camera */
static void move_camera(t_scene *s, int keycode)
{
    t_vec3 cam_dir = {0};

    if (keycode == KEY_I)         cam_dir.z =  SPEED;
    else if (keycode == KEY_K)    cam_dir.z = -SPEED;
    else if (keycode == KEY_J)    cam_dir.x = -SPEED;
    else if (keycode == KEY_L)    cam_dir.x =  SPEED;
    else if (keycode == KEY_U)    cam_dir.y =  SPEED;
    else if (keycode == KEY_O)    cam_dir.y = -SPEED;

    t_vec3 move = vec3_add(vec3_add(vec3_mult(s->camera.right,   cam_dir.x),
                                    vec3_mult(s->camera.up,      cam_dir.y)),
                                    vec3_mult(s->camera.forward, cam_dir.z));

    s->camera.origin = vec3_add(s->camera.origin, move);
}

/* Rotate selected object around Y or X axis */
static void rotate_object(t_scene *s, int keycode)
{
    float angle_deg = 0;
    int rotate_axis = 0; // 0 = none, 1 = Y, 2 = X

    if (keycode == KEY_Q) {         // rotate Y -
        angle_deg = -5.0f;
        rotate_axis = 1;
    } else if (keycode == KEY_E) {  // rotate Y +
        angle_deg = 5.0f;
        rotate_axis = 1;
    } else if (keycode == 120) {    // 'x' key
        angle_deg = -5.0f;
        rotate_axis = 2;
    } else if (keycode == 99) {     // 'c' key
        angle_deg = 5.0f;
        rotate_axis = 2;
    }

    if (rotate_axis == 0)
        return;

    if (s->selected_mesh->type == CYLINDER)
    {
        if (rotate_axis == 1)
            s->selected_mesh->cylinder.axis = vec3_normalize(rotate_y(s->selected_mesh->cylinder.axis, angle_deg));
        else if (rotate_axis == 2)
            s->selected_mesh->cylinder.axis = vec3_normalize(rotate_x(s->selected_mesh->cylinder.axis, angle_deg));
    }
    else if (s->selected_mesh->type == PLANE)
    {
        if (rotate_axis == 1)
            s->selected_mesh->plane.normal = vec3_normalize(rotate_y(s->selected_mesh->plane.normal, angle_deg));
        else if (rotate_axis == 2)
            s->selected_mesh->plane.normal = vec3_normalize(rotate_x(s->selected_mesh->plane.normal, angle_deg));
    }
}
void update_camera_target(t_camera *cam, float pitch, float yaw)
{
    float rad_pitch = pitch * M_PI / 180.0f;
    float rad_yaw   = yaw * M_PI / 180.0f;

    t_vec3 direction = {
        cosf(rad_pitch) * sinf(rad_yaw),  // right
        sinf(rad_pitch),                  // up
        -cosf(rad_pitch) * cosf(rad_yaw)  // -forward  (now negative Z)
    };

    cam->target = vec3_add(cam->origin, direction);
}


void rotate_camera(t_scene *s, int keycode)
{
    float *pitch = &s->camera.pitch;
    float *yaw   = &s->camera.yaw;

    if (keycode == KEY_UP)    *pitch += 3.0f;
    else if (keycode == KEY_DOWN)  *pitch -= 3.0f;
    else if (keycode == KEY_LEFT)  *yaw  += 3.0f;
    else if (keycode == KEY_RIGHT) *yaw  -= 3.0f;

    *pitch = fmaxf(-89.0f, fminf(89.0f, *pitch));

    update_camera_target(&s->camera, *pitch, *yaw);
    compute_camera_basis(&s->camera);
}
 

int key_hook(int keycode, void *scene)
{
    t_scene *s = (t_scene *)scene;

    printf("Key pressed: %d\n", keycode);

    if (keycode == 65307)
        exit(0);

    if (s->selected_mesh)
    {
        move_object(s, keycode);
        rotate_object(s, keycode);
    }
    rotate_camera(s, keycode);
    move_camera(s, keycode);
    render(s);
    return 0;
}
