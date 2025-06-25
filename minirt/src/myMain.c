#include <mlx.h>
#include <stdlib.h>
#include <stdio.h>
#include "../includes/scene_math.h"


#define WIDTH 1920
#define HEIGHT 1080

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




int main(int argc, char **argv) {
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
    
    // Camera
    double focal_length = 1.0;
    t_vec3 focal_vec = {0 , 0 , focal_length};    
    double viewport_height = 2.0;
    double viewport_width = viewport_height * ((double)WIDTH / (double)HEIGHT);

    t_vec3 camera_center = {0, 0, 0};

    // Viewport axes (u = horizontal, v = vertical)
    t_vec3 viewport_u = {viewport_width, 0, 0};
    t_vec3 viewport_v = {0, -viewport_height, 0}; // Minus to make image right-side up

    // Per-pixel steps
    t_vec3 pixel_delta_u = vec3_div(viewport_u, WIDTH);
    t_vec3 pixel_delta_v = vec3_div(viewport_v, HEIGHT);

    // Upper-left corner of viewport
    t_vec3 viewport_upper_left = vec3_sub(
        vec3_sub(
            vec3_sub(camera_center, focal_vec),
            vec3_div(viewport_u, 2)
        ),
        vec3_div(viewport_v, 2)
    );

    // Center of pixel (0,0)
    t_vec3 pixel00_loc = vec3_add(
        viewport_upper_left,
        vec3_mult(vec3_add(pixel_delta_u, pixel_delta_v), 0.5)
    );

    printf("Pixel (0,0) center at: (%.2f, %.2f, %.2f)\n",
        pixel00_loc.x, pixel00_loc.y, pixel00_loc.z);

    // Render 
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {

            t_vec3 p_center = vec3_add (
                pixel00_loc,
                vec3_add (
                    vec3_mult(pixel_delta_u, i),
                    vec3_mult(pixel_delta_v, j)
                )
            );

            t_vec3 ray_direction = vec3_sub(p_center, camera_center);
            t_ray ray = (t_ray){camera_center, ray_direction};

            t_vec3 pixel_color = ray_color(ray);
            int color = rgb_to_int(pixel_color.x, pixel_color.y, pixel_color.z);
            write_color(addr, line_len, bpp, j, i, color);
        }
    }

    mlx_put_image_to_window(mlx, win, img, 0, 0);
    mlx_loop(mlx);
    return (0);
}
