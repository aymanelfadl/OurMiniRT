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

    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            double r = (double)i / (WIDTH - 1);
            double g = (double)j / (HEIGHT - 1);
            double b = 0.0;
            int color = rgb_to_int(r, g, b);
            write_color(addr, line_len, bpp, j, i, color);
        }
    }
    
    // Camera
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (WIDTH / HEIGHT);
    t_vec3 camera_center = {0, 0, 0};

    t_vec3 viewport_u = {viewport_width, 0, 0};
    t_vec3 viewport_v = {0, -viewport_height, 0};


    mlx_put_image_to_window(mlx, win, img, 0, 0);
    mlx_loop(mlx);
    return (0);
}
