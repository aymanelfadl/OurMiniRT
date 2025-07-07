# ifndef CAMERA_H
# define CAMERA_H

# include "minirt.h"
# include "math_utils.h"

t_camera init_camera(t_vec3 origin, t_vec3 target, double fov_deg, double aspect_ratio);
void compute_camera_basis(t_camera *cam);
void setup_viewport(t_camera *cam);
t_vec3 get_ray_direction(t_camera *cam, int i, int j, int width, int height);

#endif
