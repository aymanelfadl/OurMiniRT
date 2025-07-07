#ifndef MINIRT_APP_H
# define MINIRT_APP_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <float.h>
# include <limits.h>
# include <math.h>
# include <mlx.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>


# define SPHERE 1
# define PLANE 2
# define CYLINDER 3
# define CONE 4
# define MAX_OBJECTS 100


# define TRUE 1
# define FALSE 0

# define WIDTH 800
# define HEIGHT 600
# define WINDOW_NAME_RT "miniRT"



/* Error codes */
# define ERR_ARGS "Error: Invalid number of arguments\n"
# define ERR_FILE "Error: Could not open file\n"
# define ERR_FORMAT "Error: Invalid file format\n"
# define ERR_SCENE "Error: Invalid scene configuration\n"
# define ERR_MEMORY "Error: Memory allocation failed\n"
# define ERR_FILE_FORMAT "Error: File must have .rt extension\n"



// Image handling
typedef struct s_image {
    void *img;
    char *addr;
    int bpp;
    int line_len;
    int endian;
    int width;
    int height;
} t_image;

// --- Vector types (move up) ---
typedef struct s_vec3
{
    double x;
    double y;
    double z;
} t_vec3;

typedef t_vec3 t_point3;
typedef t_vec3 t_color3;

// --- Camera struct (now after t_vec3) ---
typedef struct s_camera {
    t_vec3 origin;
    t_vec3 target;
    t_vec3 forward;
    t_vec3 right;
    t_vec3 up;
    double fov_deg;
    double viewport_width;
    double viewport_height;
    double aspect_ratio;
    double focal_length;
} t_camera;

/* Main program variables structure */
typedef struct s_vars
{
	void				*mlx;
	void				*win;
	t_image				*img;
}						t_vars;


typedef struct s_quadratic
{
	double			a;
	double			b;
	double			c;
}					t_quadratic;


typedef struct s_ambient
{
	double			ratio;
	t_vec3		    color;
}					t_ambient;

typedef struct s_light
{
	t_point3		position;
	double			brightness;
	t_vec3		    color;
}					t_light;

typedef struct s_material
{
	t_vec3		    color;
}					t_material;

typedef struct s_sphere
{
	t_point3		center;
	double			diameter;
	t_vec3		    color;
	t_material		material;
}					t_sphere;

typedef struct s_plane
{
	t_point3		point;
	t_vec3			normal;
	t_vec3		    color;
	t_material		material;
}					t_plane;

typedef struct s_cylinder
{
	t_point3		center;
	t_vec3			axis;
	double			diameter;
	double			height;
	t_vec3		    color;
	t_material		material;
}					t_cylinder;

typedef struct s_cone
{
	t_point3		vertex;
	t_vec3			axis;
	double			angle;
	double			height;
	t_vec3		    color;
	t_material		material;
}					t_cone;

// --- Ray type ---
typedef struct s_ray
{
	t_vec3			origin;
	t_vec3			direction;
}					t_ray;

#endif