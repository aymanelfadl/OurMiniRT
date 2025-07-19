#include "minirt.h"

int ray_plane(const t_ray *r, const t_plane *pl, t_hit *h)
{
    float denom = vec3_dot(pl->normal, r->direction);

    if (fabsf(denom) < 1e-6f)
        return 0;
    t_vec3 diff = vec3_sub(pl->point, r->origin);

    float t = vec3_dot(diff, pl->normal) / denom;
    if (t < 0.f)
        return 0;
    
    h->t = t;
    h->p = vec3_add(r->origin, vec3_mult(r->direction, t));
    h->n = (denom > 0) ? vec3_mult(pl->normal, -1.f) : pl->normal;
    h->color = pl->color;
    
    return 1;
}