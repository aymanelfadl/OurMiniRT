#ifndef PARSER_H
#define PARSER_H

#include "minirt.h"

typedef struct s_token {
    char *id;
    char *args;
} t_token;

// Parser utility functions
int array_len(char **s);
char *ft_strjoin_array(char const **arr, char const *sep);
int ft_strcmp(const char *s1, const char *s2);
void free_token(void *ptr);
void ft_free_split(char **tab);
double ft_atof(const char *str);
int array_len_elements(char **s);


#endif