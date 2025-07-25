#ifndef MLX_HOOKS_H
#define MLX_HOOKS_H


#define SPEED 0.5f
#define KEY_W       119  // 'w'
#define KEY_S       115  // 's'
#define KEY_A       97   // 'a'
#define KEY_D       100  // 'd'
#define KEY_PLUS    61   // '+'
#define KEY_MINUS   45   // '-'
#define KEY_Q       113  // 'q'
#define KEY_E       101  // 'e'

#define KEY_I       105  // 'i'
#define KEY_K       107  // 'k'
#define KEY_J       106  // 'j'
#define KEY_L       108  // 'l'
#define KEY_U       117  // 'u'
#define KEY_O       111  // 'o'

#define KEY_LEFT    65361  // ←
#define KEY_RIGHT   65363  // →
#define KEY_UP      65362  // ↑
#define KEY_DOWN    65364  // ↓

#define KEY_F       102
#define KEY_R       114

int mouse_move_hook(int keycode ,int x, int y, void *param);
void trace_mouse_ray(t_scene *scene, t_ray *ray);


int key_hook(int keycode, void *scene);

// objects hooks :
void rotate_objects(t_scene *s, int keycode);
void move_object(t_scene *s, int keycode);
void scale_object(t_scene *s, int keycode);

// camera hooks : 
void rotate_camera(t_scene *s, int keycode);
void move_camera(t_scene *s, int keycode);



#endif