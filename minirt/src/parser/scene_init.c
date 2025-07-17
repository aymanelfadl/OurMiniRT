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


void fill_scene(t_scene *scene, t_token *token)
{
    if (!ft_strcmp(token->id, "C"))
    {
        printf("token args: %s\n", token->args);
        t_camera *cam = parse_camera(token->args);
        if (cam) 
        {
            scene->camera = *cam;
            printf("cam: (%.2f, %.2f, %.2f) and fov : %.2f\n", scene->camera.origin.x,
                    scene->camera.origin.y,
                    scene->camera.origin.z,
                    scene->camera.fov_deg);
            free(cam); // free temp malloc
        }
        else
        {
            printf("Err\n");
        }
    }
}



t_scene *build_scene(int fd)
{
    char    *line;
    t_list  *node;
    t_scene *scene;

    scene = ft_calloc(1, sizeof(t_scene));
    if (!scene)
        return (NULL);
    line = get_next_line(fd);
    while (line != NULL)
    {
        node = creat_token(line);
        if (node)
        {
            fill_scene(scene, (t_token *)node->content);
            ft_lstdelone(node, free_token);
        }
        line = get_next_line(fd);
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
    scene->vars.mlx  = mlx_init();
    scene->vars.win  = mlx_new_window(scene->vars.mlx, WIDTH, HEIGHT, "MiniRT");
    scene->image     = init_image(scene->vars.mlx, WIDTH, HEIGHT);
    scene->camera    = init_camera((t_vec3){0, 0, 0},
                                   (t_vec3){0, 0, 1},
                                   90.0,
                                   (double)WIDTH / HEIGHT);
    return (scene);
}