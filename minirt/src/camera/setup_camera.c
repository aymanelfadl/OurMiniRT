#include "minirt.h"
#include "camera.h"

t_camera init_camera(t_vec3 origin, t_vec3 target, double fov_deg, double aspect_ratio) {
    t_camera cam;
    cam.origin = origin;
    cam.target = target;
    cam.fov_deg = fov_deg;
    cam.aspect_ratio = aspect_ratio;
    cam.focal_length = 1.0;
    return cam;
}

void compute_camera_basis(t_camera *cam) {
    t_vec3 world_up = (t_vec3){0, 1, 0};

    cam->forward = vec3_normalize(vec3_sub(cam->target, cam->origin));
    cam->right = vec3_normalize(vec3_cross(world_up, cam->forward));
    cam->up = vec3_cross(cam->forward, cam->right);
}

void setup_viewport(t_camera *cam) {
    double theta = cam->fov_deg * M_PI / 180.0;
    cam->viewport_height = 2.0 * tan(theta / 2.0);
    cam->viewport_width = cam->viewport_height * cam->aspect_ratio;
}

t_vec3 get_ray_direction(t_camera *cam, int i, int j, int width, int height) {
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

