#include "minirt.h"

int rgb_to_int(double r, double g, double b)
{
    int ir = (int)(255.999 * fmin(r, 1.0));
    int ig = (int)(255.999 * fmin(g, 1.0));
    int ib = (int)(255.999 * fmin(b, 1.0));
    return (ir << 16) | (ig << 8) | ib;
}

t_vec3 compute_lighting(t_point3 hit_point, t_vec3 normal, t_vec3 object_color, t_scene *scene)
{
    /* === Ambient === */
    t_vec3 ambient = vec3_mul_vec(object_color, scene->ambient.color);
    ambient = vec3_mult(ambient, scene->ambient.ratio / 255.0);

    /* === Diffuse === */
    t_vec3 to_light = vec3_normalize(vec3_sub(scene->light.position, hit_point));
    float diffuse_factor = fmaxf(vec3_dot(normal, to_light), 0.0f);
    t_vec3 diffuse = vec3_mul_vec(object_color, scene->light.color);
    diffuse = vec3_mult(diffuse, (scene->light.brightness * diffuse_factor) / 255.0);

    /* === Final Color === */
    return vec3_add(ambient, diffuse);
}