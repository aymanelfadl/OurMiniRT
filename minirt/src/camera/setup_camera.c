#include "minirt.h"

void compute_camera_basis(t_camera *cam)
{
    cam->forward.x = cosf(cam->pitch) * sinf(cam->yaw);
    cam->forward.y = sinf(cam->pitch);
    cam->forward.z = -cosf(cam->pitch) * cosf(cam->yaw);
    cam->forward = vec3_normalize(cam->forward);
    
    // Safer world up selection
    t_vec3 world_up = fabs(cam->forward.y) > 0.999f  ? (t_vec3){0, 0, 1} : (t_vec3){0, 1, 0};
    
    // Step 3: Build right and up (choose consistent cross order)
    cam->right = vec3_normalize(vec3_cross(world_up, cam->forward));
    cam->up    = vec3_cross(cam->forward, cam->right);
}

void setup_viewport(t_camera *cam)
{
    double theta;

    theta = cam->fov_deg * M_PI / 180.0;
    cam->viewport_height = 2.0 * tan(theta / 2.0);
    cam->viewport_width = cam->viewport_height * cam->aspect_ratio;
}

t_vec3 get_ray_direction(t_camera *cam, int i, int j, int width, int height)
{
    double u = (double)i / (width - 1);
    double v = (double)j / (height - 1);

    double x = (2.0 * u - 1.0) * cam->viewport_width / 2.0;
    double y = (1.0 - 2.0 * v) * cam->viewport_height / 2.0;

    t_vec3 direction = vec3_add(
        vec3_add(
            vec3_mult(cam->right, x),
            vec3_mult(cam->up, y)
        ),
        vec3_mult(cam->forward, cam->focal_length)
    );

    return vec3_normalize(direction);
}

