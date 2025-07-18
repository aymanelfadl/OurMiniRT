#include "parser.h"

t_list *creat_token(char *line)
{
    char   **split;
    t_token *tok;

    split = ft_split(line, "\t ");
    if (!split || !split[0])
        return (NULL);
    tok = malloc(sizeof(t_token));
    if (!tok)
        return (NULL);
    tok->id = ft_strdup(split[0]);
    if (split[1])
        tok->args = ft_strjoin_array((const char **)(split + 1), " ");
    else
        tok->args = ft_strdup("");
    ft_free_split(split);
    return (ft_lstnew(tok));
}
t_vec3 get_coordinates(char *str)
{
    char **coords;
    t_vec3 v;

    coords = ft_split(str, ",");
    if (!coords || array_len_elements(coords) != 3)
    {
        ft_free_split(coords);
        return (t_vec3){INFINITY, INFINITY, INFINITY};
    }
    v.x = ft_atof(coords[0]);
    v.y = ft_atof(coords[1]);
    v.z = ft_atof(coords[2]);
    ft_free_split(coords);
    return v;
}

t_camera *parse_camera(char *cam_args)
{
    t_camera *cam = malloc(sizeof(t_camera));
    if (!cam)
        return NULL;

    char **content = ft_split(cam_args, " \t");
    printf("content len: %d\n", array_len_elements(content));
    if (!content || array_len_elements(content) != 3)
    {
        ft_free_split(content);
        free(cam);
        return NULL;
    }
    
    cam->origin = get_coordinates(content[0]);
    cam->target = get_coordinates(content[1]);
    cam->fov_deg = ft_atoi(content[2]);

    ft_free_split(content);
    return cam;
}

t_ambient *parse_ambient(char *amb_args)
{
    t_ambient *ambient = malloc(sizeof(t_ambient));
    if (!ambient)
        return NULL;

    char **content = ft_split(amb_args, " \t");
    if (!content || array_len_elements(content) != 2)
    {
        ft_free_split(content);
        free(ambient);
        return NULL;
    }
    
    ambient->ratio = ft_atof(content[0]);
    ambient->color = get_coordinates(content[1]);

    ft_free_split(content);
    return ambient;
}

t_light *parse_light(char *light_args)
{
    t_light *light = malloc(sizeof(t_light));
    if (!light)
        return NULL;
    
    char **content = ft_split(light_args, " \t");
    if (!content || array_len_elements(content) != 3)
    {
        ft_free_split(content);
        free(light);
        return NULL;
    }

    light->position = get_coordinates(content[0]);
    light->brightness = ft_atof(content[1]);
    light->color = get_coordinates(content[2]);

    ft_free_split(content);
    return light;
}


void fill_scene(t_scene *scene, t_token *token)
{
    if (!ft_strcmp(token->id, "C"))
    {
        t_camera *cam = parse_camera(token->args);
        if (cam) 
        {
            scene->camera = *cam;
            free(cam);
        }
        else
        {
            printf("Err\n");
        }
    }
    else if (!ft_strcmp(token->id, "A"))
    {
        t_ambient *ambient = parse_ambient(token->args);
        if (ambient) 
        {
            scene->ambient = *ambient;
            free(ambient);
        }
        else
        {
            printf("Err\n");
        }
    }
    else if (!ft_strcmp(token->id, "L"))
    {
        t_light *light = parse_light(token->args);
        if (light) 
        {
            scene->light = *light;
            free(light);
        }
        else
        {
            printf("Err\n");
        }
    }
    
}

static int id_to_idx(const char *id)
{
    if (ft_strcmp(id, "A") == 0)
        return 0;
    if (ft_strcmp(id, "C") == 0)
        return 1;
    if (ft_strcmp(id, "L") == 0)
        return 2;
    return -1;
}

static int mark_uniq(int *set, const char *id)
{
    int idx = id_to_idx(id);
    if (idx == -1)
        return 0;
    if (set[idx])
        return 1;
    set[idx] = 1;
    return 0;
}

t_scene *build_scene(int fd)
{
    char    *line;
    t_list  *node;
    t_scene *scene;
    int     uniq[3] = {0};

    scene = ft_calloc(1, sizeof(t_scene));
    if (!scene)
        return (NULL);
    line = get_next_line(fd);
    while (line != NULL)
    {
        node = creat_token(line);
        if (node)
        {
            t_token *tok = (t_token *)node->content;
            if (mark_uniq(uniq, tok->id))
            {
                ft_lstdelone(node, free_token);
                free(line);
                free(scene);
                return (NULL);
            }
            fill_scene(scene, tok);
            ft_lstdelone(node, free_token);
        }
        free(line);
        line = get_next_line(fd);
    }
    if (uniq[0] + uniq[1] + uniq[2] != 3)
    {
        free(scene);
        return (NULL);
    }
    return (scene);
}

t_scene *valide_scene(char *file)
{
    char   *suffix;
    int     fd;
    t_scene *scene;

    suffix = ft_strrchr(file, '.');
    if (!suffix || ft_strcmp(suffix, ".rt") != 0)
        return (NULL);
    fd = open(file, O_RDONLY);
    if (fd < 0)
        return (NULL);
    scene = build_scene(fd);
    close(fd);
    return (scene);
}

t_scene *scene_init(char *file)
{
    t_scene *scene;

    scene = valide_scene(file);
    // if (!scene)
    //     return (NULL);
    // scene->vars.mlx  = mlx_init();
    // scene->vars.win  = mlx_new_window(scene->vars.mlx, WIDTH, HEIGHT, "MiniRT");
    // scene->image     = init_image(scene->vars.mlx, WIDTH, HEIGHT);
    // scene->camera    = init_camera((t_vec3){0, 0, 0},
    //                                (t_vec3){0, 0, 1},
    //                                90.0,
    //                                (double)WIDTH / HEIGHT);
    return (scene);
}