#include "minirt.h"

int key_hook(int keycode, void *scene)
{
    t_scene *s = (t_scene *)scene;

    printf("Key pressed: %d\n", keycode);

    if (keycode == 65307)
        exit(0);

    if (s->selected_mesh)
    {
        move_object(s, keycode);
        rotate_objects(s, keycode);
        scale_object(s, keycode);
    }
    rotate_camera(s, keycode);
    move_camera(s, keycode);
    render(s);
    return 0;
}
