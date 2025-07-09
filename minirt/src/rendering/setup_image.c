#include "minirt.h"
#include "camera.h"

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
