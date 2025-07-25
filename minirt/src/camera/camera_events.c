#include "minirt.h"

static void update_camera_target(t_camera *cam, float pitch, float yaw)
{
    float rad_pitch = pitch * M_PI / 180.0f;
    float rad_yaw   = yaw * M_PI / 180.0f;

    t_vec3 direction = {
        cosf(rad_pitch) * sinf(rad_yaw),
        sinf(rad_pitch),
        -cosf(rad_pitch) * cosf(rad_yaw)
    };
    cam->target = vec3_add(cam->origin, direction);
}

void rotate_camera(t_scene *s, int keycode)
{
    if (keycode == KEY_UP)
        s->camera.pitch += 3.0f;
    else if (keycode == KEY_DOWN)
        s->camera.pitch -= 3.0f;
    else if (keycode == KEY_LEFT)
        s->camera.yaw  += 3.0f;
    else if (keycode == KEY_RIGHT)
        s->camera.yaw  -= 3.0f;
    update_camera_target(&s->camera, s->camera.pitch, s->camera.yaw);
    compute_camera_basis(&s->camera);
}

void move_camera(t_scene *s, int keycode)
{
    t_vec3 cam_dir = {0};

    if (keycode == KEY_I)
        cam_dir.z =  SPEED;
    else if (keycode == KEY_K)
        cam_dir.z = -SPEED;
    else if (keycode == KEY_J)
        cam_dir.x = -SPEED;
    else if (keycode == KEY_L)
        cam_dir.x =  SPEED;
    else if (keycode == KEY_U)
        cam_dir.y =  SPEED;
    else if (keycode == KEY_O)
        cam_dir.y = -SPEED;

    t_vec3 move = vec3_add(vec3_add(vec3_mult(s->camera.right,   cam_dir.x),
                                    vec3_mult(s->camera.up,      cam_dir.y)),
                                    vec3_mult(s->camera.forward, cam_dir.z));
    s->camera.origin = vec3_add(s->camera.origin, move);
}
