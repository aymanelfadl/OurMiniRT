#include "minirt.h"
#include "camera.h"
#include "math_utils.h"
#include "mlx_init.h"

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
    double denom = vec3_dot(normal, ray.direction);
    if (fabs(denom) < 1e-6) return -1;
    double t = vec3_dot(vec3_sub(point, ray.origin), normal) / denom;
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

            t = hit_plane((t_vec3){0,0,0}, (t_vec3){0,1,0}, ray); // floor
            if (t > 0 && t < t_min) { t_min = t; color = (t_vec3){0.6, 0.2, 0.2}; }
            t = hit_plane((t_vec3){0,6,0}, (t_vec3){0,-1,0}, ray); // ceiling
            if (t > 0 && t < t_min) { t_min = t; color = (t_vec3){0.8, 0.8, 0.8}; }
            t = hit_plane((t_vec3){-3,0,0}, (t_vec3){1,0,0}, ray); // left
            if (t > 0 && t < t_min) { t_min = t; color = (t_vec3){0.2, 0.6, 0.2}; }
            t = hit_plane((t_vec3){3,0,0}, (t_vec3){-1,0,0}, ray); // right
            if (t > 0 && t < t_min) { t_min = t; color = (t_vec3){0.6, 0.6, 0.2}; }
            t = hit_plane((t_vec3){0,0,0}, (t_vec3){0,0,-1}, ray); // back
            if (t > 0 && t < t_min) { t_min = t; color = (t_vec3){0.2, 0.6, 0.6}; }

            int rgb = rgb_to_int(color.x, color.y, color.z);
            my_mlx_pixel_put(img, i, j, rgb);
        }
    }
    mlx_put_image_to_window(scene->vars.mlx, scene->vars.win, img->img, 0, 0);
}

int handle_key(int keycode, t_scene *scene) {
    printf("Pressed keycode: %d\n", keycode);  // Add this line

    double step = 0.5;
    if (keycode == 65307) exit(0);
    if (keycode == 119) scene->camera.origin = vec3_add(scene->camera.origin, vec3_mult(scene->camera.forward, step)); // W
    if (keycode == 115) scene->camera.origin = vec3_sub(scene->camera.origin, vec3_mult(scene->camera.forward, step)); // S
    if (keycode == 97)  scene->camera.origin = vec3_sub(scene->camera.origin, vec3_mult(scene->camera.right, step));   // A
    if (keycode == 100) scene->camera.origin = vec3_add(scene->camera.origin, vec3_mult(scene->camera.right, step));   // D
    if (keycode == 65362) scene->camera.origin.y += step; // UP
    if (keycode == 65364) scene->camera.origin.y -= step; // DOWN

    compute_camera_basis(&scene->camera);
    setup_viewport(&scene->camera);
    render(scene);
    return 0;
}


int main() {
    t_scene scene;
    scene.vars.mlx = mlx_init();
    scene.vars.win = mlx_new_window(scene.vars.mlx, WIDTH, HEIGHT, "Camera Raytracer");
    scene.image = init_image(scene.vars.mlx, WIDTH, HEIGHT);

    scene.camera = init_camera((t_vec3){0, 3, 8}, (t_vec3){0, 3, 0}, 70.0, (double)WIDTH / HEIGHT);
    compute_camera_basis(&scene.camera);
    setup_viewport(&scene.camera);

    render(&scene);

    mlx_hook(scene.vars.win, 2, 1L << 0, handle_key, &scene);
    mlx_loop(scene.vars.mlx);
    return 0;
}