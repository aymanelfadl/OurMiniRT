#include "minirt.h"

void update_camera_target(t_camera *cam, float pitch, float yaw)
{
    cam->pitch = pitch;
    cam->yaw = yaw;

    cam->forward.x = cosf(cam->pitch) * sinf(cam->yaw);
    cam->forward.y = sinf(cam->pitch);
    cam->forward.z = -cosf(cam->pitch) * cosf(cam->yaw);

    cam->target = vec3_add(cam->origin, cam->forward);
}

void rotate_camera(t_camera *cam, int keycode)
{
    float delta = 3.0f * M_PI / 180.0f;

    if      (keycode == KEY_UP)    cam->pitch += delta;
    else if (keycode == KEY_DOWN)  cam->pitch -= delta;
    else if (keycode == KEY_LEFT)  cam->yaw   += delta;
    else if (keycode == KEY_RIGHT) cam->yaw   -= delta;

    update_camera_target(cam, cam->pitch, cam->yaw);
    compute_camera_basis(cam);
}

void move_camera(t_scene *s, int keycode)
{
    t_vec3 cam_dir = {0};

    if      (keycode == KEY_I) cam_dir.z = SPEED;
    else if (keycode == KEY_K) cam_dir.z = -SPEED;
    else if (keycode == KEY_J) cam_dir.x = -SPEED;
    else if (keycode == KEY_L) cam_dir.x = SPEED;
    else if (keycode == KEY_U) cam_dir.y = SPEED;
    else if (keycode == KEY_O) cam_dir.y = -SPEED;

    t_vec3 move = vec3_add(
        vec3_add(vec3_mult(s->camera.right, cam_dir.x),
                 vec3_mult(s->camera.up, cam_dir.y)),
        vec3_mult(s->camera.forward, cam_dir.z)
    );

    s->camera.origin = vec3_add(s->camera.origin, move);
    update_camera_target(&s->camera, s->camera.pitch, s->camera.yaw);
}
