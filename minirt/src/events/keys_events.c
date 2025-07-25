#include "minirt.h"

int key_hook(int keycode, void *scene)
{
    t_scene *s = (t_scene *)scene;

    printf("Key pressed: %d\n", keycode);

    if (keycode == 65307)
        exit(0);

    printf("befor any events:\n");
    print_debug(s);

    if (s->selected_mesh)
    {
        move_object(s, keycode);
        rotate_objects(s, keycode);
        scale_object(s, keycode);
    }
    rotate_camera(s, keycode);
    move_camera(s, keycode);
    render(s);
    printf("after any events:\n");
    print_debug(s);

    return 0;
}
