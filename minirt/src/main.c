#include "minirt.h"
#include "camera.h"
#include "math_utils.h"

// -------- Utilities --------
int rgb_to_int(double r, double g, double b) {
    int ir = (int)(255.999 * r);
    int ig = (int)(255.999 * g);
    int ib = (int)(255.999 * b);
    return (ir << 16) | (ig << 8) | ib;
}

void write_pixel(char *addr, int line_len, int bpp, int row, int col, int color) {
    char *dst = addr + (row * line_len + col * (bpp / 8));
    *(unsigned int *)dst = color;
}

// -------- Intersections --------
double hit_sphere(t_vec3 center, double radius, t_ray ray) {
    t_vec3 oc = vec3_sub(ray.origin, center);
    double a = vec3_dot(ray.direction, ray.direction);
    double b = 2.0 * vec3_dot(oc, ray.direction);
    double c = vec3_dot(oc, oc) - radius * radius;
    double disc = b * b - 4 * a * c;
    if (disc < 0) return -1;
    double sqrt_disc = sqrt(disc);
    double t1 = (-b - sqrt_disc) / (2 * a);
    double t2 = (-b + sqrt_disc) / (2 * a);
    if (t1 > 0.001) return t1;
    if (t2 > 0.001) return t2;
    return -1;
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

// -------- Main --------
int main() {
    int bpp, line_len, endian;
    void *mlx = mlx_init();
    void *win = mlx_new_window(mlx, WIDTH, HEIGHT, "Camera Raytracer");
    void *img = mlx_new_image(mlx, WIDTH, HEIGHT);
    char *addr = mlx_get_data_addr(img, &bpp, &line_len, &endian);

    // === Scene ===
    t_vec3 plane_point_1 = {0, 0, 0};
    t_vec3 plane_normal_1 = {0, 1, 0};     // floor, up

    t_vec3 plane_point_2 = {0, 6, 0};
    t_vec3 plane_normal_2 = {0, -1, 0};    // ceiling, down

    t_vec3 plane_point_3 = {-3, 0, 0};
    t_vec3 plane_normal_3 = {1, 0, 0};     // left wall, right

    t_vec3 plane_point_4 = {3, 0, 0};
    t_vec3 plane_normal_4 = {-1, 0, 0};    // right wall, left

    t_vec3 plane_point_5 = {0, 0, 0};
    t_vec3 plane_normal_5 = {0, 0, -1};    // back wall, toward camera

    

    // === Camera ===
    t_camera cam = init_camera((t_vec3){0, 3, 8}, (t_vec3){0, 0, 1}, 180.0, (double)WIDTH / HEIGHT);
    compute_camera_basis(&cam);
    setup_viewport(&cam);

    // === Render Loop ===
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            t_ray ray = {cam.origin, get_ray_direction(&cam, i, j, WIDTH, HEIGHT)};
            t_vec3 color = background_color(ray);
            double t_min = 1e9;

            double t;

            t = hit_plane(plane_point_1, plane_normal_1, ray);
            if (t > 0 && t < t_min) {
                t_min = t;
                color = (t_vec3){150.0 / 255.0, 50.0 / 255.0, 50.0 / 255.0};
            }

            t = hit_plane(plane_point_2, plane_normal_2, ray);
            if (t > 0 && t < t_min) {
                t_min = t;
                color = (t_vec3){200.0 / 255.0, 200.0 / 255.0, 200.0 / 255.0};
            }

            t = hit_plane(plane_point_3, plane_normal_3, ray);
            if (t > 0 && t < t_min) {
                t_min = t;
                color = (t_vec3){50.0 / 255.0, 150.0 / 255.0, 50.0 / 255.0};
            }

            t = hit_plane(plane_point_4, plane_normal_4, ray);
            if (t > 0 && t < t_min) {
                t_min = t;
                color = (t_vec3){150.0 / 255.0, 150.0 / 255.0, 50.0 / 255.0};
            }

            t = hit_plane(plane_point_5, plane_normal_5, ray);
            if (t > 0 && t < t_min) {
                t_min = t;
                color = (t_vec3){50.0 / 255.0, 150.0 / 255.0, 150.0 / 255.0};
            }

            int rgb = rgb_to_int(color.x, color.y, color.z);
            write_pixel(addr, line_len, bpp, j, i, rgb);
        }
    }


    mlx_put_image_to_window(mlx, win, img, 0, 0);
    mlx_loop(mlx);
    return 0;
}
