#include "minirt.h"
#include "camera.h"
#include "math_utils.h"
#include "mlx_init.h"

#define ROOM_MAX_X 3
#define ROOM_MAX_Y 6
#define ROOM_MAX_Z 10

int rgb_to_int(double r, double g, double b) {
    int ir = (int)(255.999 * r);
    int ig = (int)(255.999 * g);
    int ib = (int)(255.999 * b);
    return (ir << 16) | (ig << 8) | ib;
}

void my_mlx_pixel_put(t_image *img, int x, int y, int color) {
    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return;
    char *pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
    *(unsigned int *)pixel = color;
}

double hit_plane(t_vec3 point, t_vec3 normal, t_ray ray) {
    double divisor = vec3_dot(normal, ray.direction);
    if (fabs(divisor) < 1e-6)
        return -1;
    double t = vec3_dot(vec3_sub(point, ray.origin), normal) / divisor;
    return (t >= 0) ? t : -1;
}

t_vec3 background_color(t_ray ray) {
    t_vec3 dir = vec3_normalize(ray.direction);
    double t = 0.5 * (dir.y + 1.0);
    t_vec3 white = {1.0, 1.0, 1.0};
    t_vec3 blue = {0.5, 0.7, 1.0};
    return vec3_add(vec3_mult(white, 1.0 - t), vec3_mult(blue, t));
}

void render(t_scene *scene) {
    
    t_image *img = &scene->image;
    
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            t_ray ray = {
                scene->camera.origin,
                get_ray_direction(&scene->camera, i, j, HEIGHT, WIDTH)
            };
            t_vec3 color = background_color(ray);
            double t_min = 1e9, t;
            int hit_wall = 0;

            // Floor (y=0), bounded by x,z
            t = hit_plane((t_vec3){0,0,0}, (t_vec3){0,1,0}, ray);
            if (t > 0 && t < t_min) {
                t_vec3 p = vec3_add(ray.origin, vec3_mult(ray.direction, t));
                if (p.x >= -ROOM_MAX_X && p.x <= ROOM_MAX_X &&
                    p.z >= -ROOM_MAX_Z && p.z <= ROOM_MAX_Z) {
                    t_min = t;
                    color = (t_vec3){0.6, 0.2, 0.2}; // reddish floor
                    hit_wall = 1;
                }
            }

            // Ceiling (y=ROOM_MAX_Y), bounded by x,z
            t = hit_plane((t_vec3){0,ROOM_MAX_Y,0}, (t_vec3){0,-1,0}, ray);
            if (t > 0 && t < t_min) {
                t_vec3 p = vec3_add(ray.origin, vec3_mult(ray.direction, t));
                if (p.x >= -ROOM_MAX_X && p.x <= ROOM_MAX_X &&
                    p.z >= -ROOM_MAX_Z && p.z <= ROOM_MAX_Z) {
                    t_min = t;
                    color = (t_vec3){0.8, 0.8, 0.8}; // light gray ceiling
                    hit_wall = 2;
                }
            }

            // Left wall (x = -ROOM_MAX_X), bounded by y,z
            t = hit_plane((t_vec3){-ROOM_MAX_X,0,0}, (t_vec3){1,0,0}, ray);
            if (t > 0 && t < t_min) {
                t_vec3 p = vec3_add(ray.origin, vec3_mult(ray.direction, t));
                if (p.y >= 0 && p.y <= ROOM_MAX_Y &&
                    p.z >= -ROOM_MAX_Z && p.z <= ROOM_MAX_Z) {
                    t_min = t;
                    color = (t_vec3){0.2, 0.6, 0.2}; // green left wall
                    hit_wall = 3;
                }
            }

            // Right wall (x = ROOM_MAX_X), bounded by y,z
            t = hit_plane((t_vec3){ROOM_MAX_X,0,0}, (t_vec3){-1,0,0}, ray);
            if (t > 0 && t < t_min) {
                t_vec3 p = vec3_add(ray.origin, vec3_mult(ray.direction, t));
                if (p.y >= 0 && p.y <= ROOM_MAX_Y &&
                    p.z >= -ROOM_MAX_Z && p.z <= ROOM_MAX_Z) {
                    t_min = t;
                    color = (t_vec3){0.6, 0.6, 0.2}; // yellow right wall
                    hit_wall = 4;
                }
            }

            // Back wall (z = -ROOM_MAX_Z) - This should be in front of you!
            t = hit_plane((t_vec3){0,0,-ROOM_MAX_Z}, (t_vec3){0,0,1}, ray);
            if (t > 0 && t < t_min) {
                t_vec3 p = vec3_add(ray.origin, vec3_mult(ray.direction, t));
                if (p.x >= -ROOM_MAX_X && p.x <= ROOM_MAX_X &&
                    p.y >= 0 && p.y <= ROOM_MAX_Y) {
                    t_min = t;
                    color = (t_vec3){0.2, 0.6, 0.6}; // cyan back wall
                    hit_wall = 5;
                }
            }

            // Front wall (z = ROOM_MAX_Z) - This should be behind you
            t = hit_plane((t_vec3){0,0,ROOM_MAX_Z}, (t_vec3){0, 0, -1}, ray);
            if (t > 0 && t < t_min) {
                t_vec3 p = vec3_add(ray.origin, vec3_mult(ray.direction, t));
                if (p.x >= -ROOM_MAX_X && p.x <= ROOM_MAX_X &&
                    p.y >= 0 && p.y <= ROOM_MAX_Y) {
                    t_min = t;
                    color = (t_vec3){0.7, 0.4, 0.1}; // brown front wall
                    hit_wall = 6;
                }
            }

            int rgb = rgb_to_int(color.x, color.y, color.z);
            my_mlx_pixel_put(img, i, j, rgb);
        }
    }
    mlx_put_image_to_window(scene->vars.mlx, scene->vars.win, img->img, 0, 0);
}

void rotate_yaw(t_camera *cam, double angle_rad)
{
    double cos_a = cos(angle_rad);
    double sin_a = sin(angle_rad);

    t_vec3 f = cam->forward;

    // Rotate forward vector around Y axis (world up)
    double new_x = f.x * cos_a - f.z * sin_a;
    double new_z = f.x * sin_a + f.z * cos_a;

    cam->forward.x = new_x;
    cam->forward.z = new_z;
    cam->forward.y = f.y; // keep same height component

    cam->forward = vec3_normalize(cam->forward);

    // Update target point or direction as needed
    cam->target = vec3_add(cam->origin, cam->forward);

    // Recompute camera basis vectors after rotation
    compute_camera_basis(cam);
}



int handle_key(int keycode, t_scene *scene)
{
    printf("key pressed : %d\n", keycode);

    double step = 0.5;
    double step_angle = 0.5;
    if (keycode == 65307) // ESC
        exit(0);
    if (keycode == 119) // W
        scene->camera.origin = vec3_add(scene->camera.origin, vec3_mult(scene->camera.forward, step));
    if (keycode == 115) // S
        scene->camera.origin = vec3_sub(scene->camera.origin, vec3_mult(scene->camera.forward, step));
    if (keycode == 97)  // A
        scene->camera.origin = vec3_sub(scene->camera.origin, vec3_mult(scene->camera.right, step));
    if (keycode == 100) // D
        scene->camera.origin = vec3_add(scene->camera.origin, vec3_mult(scene->camera.right, step));
    if (keycode == 65362) // UP arrow
        scene->camera.origin.y += step;
    if (keycode == 65364) // DOWN arrow
        scene->camera.origin.y -= step;
    if (keycode == 65361) // LEFT arrow
        rotate_yaw(&scene->camera, step_angle);
    if (keycode == 65363) // RIGHT arrow
        rotate_yaw(&scene->camera, -step_angle);

    
    compute_camera_basis(&scene->camera);
    setup_viewport(&scene->camera);
    render(scene);
    return 0;
}

t_scene scene_init(void) {
    t_scene scene;

    scene.vars.mlx = mlx_init();
    scene.vars.win = mlx_new_window(scene.vars.mlx, WIDTH, HEIGHT, "MiniRT Room");
    scene.image = init_image(scene.vars.mlx, WIDTH, HEIGHT);
    scene.camera = init_camera((t_vec3){0, 3, 0}, (t_vec3){0, 3, 10}, 90.0, (double)WIDTH / HEIGHT);

    return scene;
}

int main()
{
    t_scene scene = scene_init();

    compute_camera_basis(&scene.camera);
    setup_viewport(&scene.camera);
    render(&scene);

    mlx_hook(scene.vars.win, 2, 1L << 0, handle_key, &scene);
    mlx_loop(scene.vars.mlx);

    return 0;
}
