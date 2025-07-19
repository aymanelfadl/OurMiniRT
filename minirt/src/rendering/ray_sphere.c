#include "minirt.h"

int ray_sphere(t_ray *r, t_sphere *s, t_hit *h)
{
    t_vec3 oc = vec3_sub(r->origin, s->center);
    
    float a = vec3_dot(r->direction, r->direction);
    float b = 2.f * vec3_dot(oc, r->direction);
    float c = vec3_dot(oc, oc) - s->radius * s->radius;

    float disc = b * b - 4.f * a * c;
    if (disc < 0.f)
        return 0;
    float t = (-b - sqrtf(disc)) / (2.f * a);
    
    if (t < 0.f)
        t = (-b + sqrtf(disc)) / (2.f * a);
    
    if (t < 0.f)
        return 0;

    h->t = t;
    h->p = vec3_add(r->origin, vec3_mult(r->direction, t));
    h->n = vec3_normalize(vec3_sub(h->p, s->center));
    h->color = s->color;

    return 1;
}