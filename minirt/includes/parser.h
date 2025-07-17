#ifndef PARSER_H
#define PARSER_H

#include "minirt.h"
#include "mlx_init.h"
#include "camera.h"
# include "../libft/libft.h"

typedef enum e_type {
    A,
    L,
    C,s
    SP,
    PL,
    CY,
} t_type;

typedef struct s_token {
    char *id;       /* "A", "C", "sp", "pl", "cy" */
    char *args;
} t_token;

t_scene *scene_init(char *file);

// Utils 

size_t total_len_array(char const **s);
char *ft_strjoin_array(char const **arr, char const *sep);
int	ft_strcmp(const char *s1, const char *s2);
void free_token(void *ptr);
void ft_free_split(char **tab);
double ft_atof(const char *str);


#endif