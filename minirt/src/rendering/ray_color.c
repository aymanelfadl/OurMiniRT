#include "minirt.h"

int rgb_to_int(double r, double g, double b)
{
    /* 3. Clamp colour and pack into 0x00RRGGBB. */
    int ir = (int)fminf(255, fmaxf(0, r));
    int ig = (int)fminf(255, fmaxf(0, g));
    int ib = (int)fminf(255, fmaxf(0, b));
    return (ir << 16) | (ig << 8) | ib;
}

t_vec3 compute_lighting(t_hit h, t_scene *scene)
{
    /* === Ambient === */
    t_vec3 ambient = vec3_mul_vec(h.color, scene->ambient.color);
    ambient = vec3_mult(ambient, scene->ambient.ratio / 255.0);

    /* === Shadow Ray === */
    t_vec3 to_light = vec3_sub(scene->light.position, h.p);
    float light_distance = vec3_length(to_light);
    to_light = vec3_normalize(to_light);

    t_ray shadow_ray;
    shadow_ray.origin = vec3_add(h.p, vec3_mult(h.n, 1e-6));
    shadow_ray.direction = to_light;

    t_hit hit_p = hit_scene(scene->meshes, &shadow_ray);
    if (hit_p.t > 1e-6 && hit_p.t < light_distance)
        return ambient;
     
    /* === Diffuse === */
    float diffuse_factor = fmaxf(vec3_dot(h.n, to_light), 0.0f);
    t_vec3 diffuse = vec3_mul_vec(h.color, scene->light.color);
    diffuse = vec3_mult(diffuse, (scene->light.brightness * diffuse_factor) / 255.0);

    /* === Final Color === */
    return vec3_add(ambient, diffuse);
}

