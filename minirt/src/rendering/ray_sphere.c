#include "minirt.h"

int ray_sphere(t_ray *r, t_sphere *s, t_hit *h)
{
    t_vec3 oc = vec3_sub(r->origin, s->center);
    
    float a = vec3_dot(r->direction, r->direction);
    float b = 2.f * vec3_dot(oc, r->direction);
    float c = vec3_dot(oc, oc) - s->radius * s->radius;

    float delta = b * b - 4.f * a * c;
    if (delta < 0.f)
        return 0;
    float t1 = (-b - sqrtf(delta)) / (2.f * a);
    float t2 = (-b + sqrtf(delta)) / (2.f * a);

    float t = INFINITY;
    if (t1 > 0.001f && t1 < t)
        t = t1;
    if (t2 > 0.001f && t2 < t)
        t = t2;

    if (t == INFINITY)
        return 0;

    h->t = t;
    h->p = vec3_add(r->origin, vec3_mult(r->direction, h->t));
    h->n = vec3_normalize(vec3_sub(h->p, s->center));
    h->color = s->color;

    return 1;
}
