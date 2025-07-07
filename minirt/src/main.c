#include <mlx.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../includes/scene_math.h"

# define WIDTH 1920
# define HEIGHT 1080
# define M_PI 3.14159265358979323846


int rgb_to_int(double r, double g, double b) {
    int ir = (int)(255.999 * r);
    int ig = (int)(255.999 * g);
    int ib = (int)(255.999 * b);
    return (ir << 16) | (ig << 8) | ib;
}

void write_color(char *addr, int line_len, int bpp, int j, int i, int color) {
    char *dst = addr + (j * line_len + i * (bpp / 8));
    *(unsigned int *)dst = color;
}

double hit_sphere(t_vec3 center, double radius, t_ray ray)
{
    t_vec3 oc = vec3_sub(ray.origin, center);
    
    double a = vec3_dot(ray.direction, ray.direction);
    double b = 2.0 * vec3_dot(oc, ray.direction);
    double c = vec3_dot(oc, oc) - radius * radius;
    
    double discriminant = b*b - 4*a*c;

    if (discriminant < 0)
        return -1.0;
    
    // Try both solutions
    double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    double t2 = (-b + sqrt(discriminant)) / (2.0 * a);
    
    // Return the closest positive intersection
    if (t1 > 0.001) return t1;
    if (t2 > 0.001) return t2;
    
    return -1.0;
}

double hit_plane(t_vec3 p0, t_vec3 normal, t_ray ray) {
    double denom = vec3_dot(normal, ray.direction);
    if (fabs(denom) < 1e-6)
        return -1;

    t_vec3 p0l0 = vec3_sub(p0, ray.origin);
    double t = vec3_dot(p0l0, normal) / denom;

    return (t >= 0) ? t : -1;
}

t_vec3 ray_color(t_ray ray) {
    t_vec3 unit_direction = vec3_normalize(ray.direction);
    double t = 0.5 * (unit_direction.y + 1.0);
    t_vec3 white = {1.0, 1.0, 1.0};
    t_vec3 blue = {0.5, 0.7, 1.0};
    return vec3_add(
        vec3_mult(white, 1.0 - t),  // Fixed: was 1.0, should be 1.0 - t
        vec3_mult(blue, t)
    );
}

double clamp(double x) {
    if (x < 0.0) return 0.0;
    if (x > 1.0) return 1.0;
    return x;
}

int main(int argc, char **argv)
{
    (void) argv;
    (void) argc;

    void    *mlx;
    void    *win;
    void    *img;
    char    *addr;
    int     bpp, line_len, endian;

    mlx = mlx_init();
    win = mlx_new_window(mlx, WIDTH, HEIGHT, "Gradient");
    img = mlx_new_image(mlx, WIDTH, HEIGHT);
    addr = mlx_get_data_addr(img, &bpp, &line_len, &endian);
    
    t_vec3 camera_center = {0, 0, 0};

    double fov = 70.0;
    double fov_rad = fov * M_PI / 180.0;
    double image_plane_z = -1.0 / tan(fov_rad / 2.0);

    t_vec3 forward = {0, 0, -1};
    t_vec3 up_gess = {0, 1, 0};

    t_vec3 u = vec3_normalize(vec3_cross(up_gess, forward));
    t_vec3 v = vec3_normalize(vec3_cross(forward, u));

    // Position objects closer and more visible
    t_vec3 plane_point = {0, -2, 0};        // Ground plane below camera
    t_vec3 plane_normal = {0, 1, 0};

    // Add a sphere right in front of camera
    t_vec3 sphere_center = {0, 0, 5};      // Directly in front
    double sphere_radius = 1.0;

    // Render with debug output
    printf("Starting render...\n");
    for (int j = 0; j < HEIGHT; j++)
    {
        if (j % (HEIGHT/10) == 0) {
            printf("Rendering row %d/%d\n", j, HEIGHT);
        }
        for (int i = 0; i < WIDTH; i++) 
        {
            double u_scalar = (i + 0.5) / (double)WIDTH;
            double v_scalar = (j + 0.5) / (double)HEIGHT;

            double x = 2.0 * u_scalar - 1.0;
            double y = 1.0 - 2.0 * v_scalar;

            t_vec3 image_plane_center = vec3_add(camera_center, vec3_mult(forward, image_plane_z));

            t_vec3 pixel_on_plane = vec3_add(
                vec3_add(
                    image_plane_center,
                    vec3_mult(u, x)),
                vec3_mult(v, y)
            );

            t_vec3 ray_dir = vec3_normalize(vec3_sub(pixel_on_plane, camera_center));
            t_ray ray = {camera_center, ray_dir};

            t_vec3 color = ray_color(ray);

            
            // Check sphere first (closer objects first)
            double t_sphere = hit_sphere(sphere_center, sphere_radius, ray);
            if (t_sphere > 0.0) {
                color = (t_vec3){0.8,0.3,0.3};  // Red sphere
            } else {
                // Check plane
                double t_plane = hit_plane(plane_point, plane_normal, ray);
                if (t_plane > 0.0) {
                    // Simple checkerboard pattern for the plane
                    t_vec3 hit_point = vec3_add(ray.origin, vec3_mult(ray.direction, t_plane));
                    int checker_x = (int)(hit_point.x + 1000) % 2;  // +1000 to handle negatives
                    int checker_z = (int)(hit_point.z + 1000) % 2;
                    if ((checker_x + checker_z) % 2 == 0) {
                        color = (t_vec3){0.8, 0.8, 0.8};  // Light gray
                    } else {
                        color = (t_vec3){0.3, 0.3, 0.3};  // Dark gray
                    }
                }
            }

            int rgb = rgb_to_int(color.x, color.y, color.z);
            write_color(addr, line_len, bpp, j, i, rgb);
        }
    }

    mlx_put_image_to_window(mlx, win, img, 0, 0);
    mlx_loop(mlx);
    return (0);
}