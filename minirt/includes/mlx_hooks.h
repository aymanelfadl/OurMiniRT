#ifndef MLX_HOOKS_H
#define MLX_HOOKS_H


int mouse_move_hook(int keycode ,int x, int y, void *param);
void trace_mouse_ray(t_scene *scene, t_ray *ray);

#endif