#include "minirt.h"


void my_mlx_pixel_put(t_image *img, int x, int y, int color)
{
    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return;
    char *pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
    *(unsigned int *)pixel = color;
}

t_image init_image(void *mlx, int width, int height)
{
    t_image img;

    img.img = mlx_new_image(mlx, width, height);
    if (!img.img)
    {
        ft_putstr_fd("Error: Failed to create image\n", 2);
        exit(1);
    }

    img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);
    img.width = width;
    img.height = height;

    return img;
}
