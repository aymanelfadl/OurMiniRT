#include <mlx.h>
#include <stdlib.h>
#include <stdio.h>
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



double hit_sphere(t_vec3 center, double raduis, t_ray ray)
{
    t_vec3 oc = vec3_sub(center, ray.origin);
    
    double a = vec3_dot(ray.direction, ray.direction);
    double b = -2.0 * vec3_dot(ray.direction, oc);
    double c = vec3_dot(oc, oc) - raduis * raduis;
    
    double D = b*b - 4*a*c;

    if (D < 0)
        return -1;
    else
        return ((-b - sqrt(D)) / 2.0 * a);
}


t_vec3 ray_color(t_ray ray) {
    double t = hit_sphere((t_vec3){0, 0, -1}, 0.5, ray);
    
    if (t > 0.0) {
        t_vec3 hit_point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
        t_vec3 normal = vec3_normalize(vec3_sub(hit_point, (t_vec3){0, 0, -1}));
        return vec3_mult(vec3_add(normal, (t_vec3){1, 1, 1}), 0.5);
    }

    t_vec3 unit_direction = vec3_normalize(ray.direction);
    t = 0.5 * (unit_direction.y + 1.0);
    t_vec3 white = {1.0, 1.0, 1.0};
    t_vec3 blue = {0.5, 0.7, 1.0};
    return vec3_add(
        vec3_mult(white, 1.0 - t),
        vec3_mult(blue, t)
    );
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
    

    t_vec3 camera_center = {0, 3, 8};

    double fov = 70.0;
    double fov_rad = fov * M_PI / 180.0;
    double image_plane_z = -1.0 / tan(fov_rad / 2.0);

    t_vec3 forward = {0, 0, -1};
    t_vec3 up_gess = {0, 1, 0}; // up of my world


    // Render 
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {

        }
    }

    mlx_put_image_to_window(mlx, win, img, 0, 0);
    mlx_loop(mlx);
    return (0);
}
