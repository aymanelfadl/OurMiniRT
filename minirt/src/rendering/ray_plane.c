#include "minirt.h"

int ray_plane(t_ray *r, t_plane *pl, t_hit *h)
{
    float lkahrij = vec3_dot(pl->normal, r->direction);

    if (fabsf(lkahrij) == 0.f)
        return 0;

    t_vec3 diff = vec3_sub(pl->point, r->origin);

    float t = vec3_dot(diff, pl->normal) / lkahrij;
    if (t < 0.f)
        return 0;
    
    h->t = t;
    h->p = vec3_add(r->origin, vec3_mult(r->direction, t));
    h->n = (lkahrij > 0) ? vec3_mult(pl->normal, -1.f) : pl->normal;
    h->color = pl->color;
    
    return 1;
}