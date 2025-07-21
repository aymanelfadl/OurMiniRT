#include "minirt.h"

t_vec3	cylinder_surface_normal(const t_cylinder *cylinder, t_point3 point)
{
	double		m;
	t_vec3		axis_projection;
	t_point3	axis_point;

	m = vec3_dot(vec3_sub(point, cylinder->center), cylinder->axis);
	axis_projection = vec3_mult(cylinder->axis, m);
	axis_point = vec3_add(cylinder->center, axis_projection);
	return (vec3_normalize(vec3_sub(point, axis_point)));
}

static int	check_cap_hit(const t_cylinder *cyl, t_ray ray, t_hit *hit,
		double height)
{
	double		denom;
	double		t;
	t_point3	cap_center;
	t_vec3		radial;

	denom = vec3_dot(cyl->axis, ray.direction);
	if (fabs(denom) < 0.0001)
		return (0);
	cap_center = vec3_add(cyl->center, vec3_mult(cyl->axis, height));
	t = vec3_dot(vec3_sub(cap_center, ray.origin), cyl->axis) / denom;
	if (t <= 0.001 || (hit->t >= 0 && t >= hit->t))
		return (0);
	radial = vec3_sub(vec3_add(ray.origin, vec3_mult(ray.direction, t)), cap_center);
	radial = vec3_sub(radial, vec3_mult(cyl->axis, vec3_dot(radial,
					cyl->axis)));
	if (vec3_length(radial) > cyl->diameter / 2.0)
		return (0);
	hit->t = t;
	hit->p = vec3_add(ray.origin, vec3_mult(ray.direction, t));
    hit->color = cyl->color;
	if ((height > 0))
		hit->n = cyl->axis;
	else
		hit->n = vec3_mult(cyl->axis, -1.f);
    return (1);
}

static int	check_cylinder_surface(const t_cylinder *cylinder, t_ray ray,
		t_hit *hit, double t)
{
	t_point3	point;
	double		m;

	point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
	m = vec3_dot(vec3_sub(point, cylinder->center), cylinder->axis);
	if (m < 0 || m > cylinder->height)
		return (0);
	hit->t = t;
	hit->p = point;
	hit->n = cylinder_surface_normal(cylinder, point);
	hit->color = cylinder->color;
	return (1);
}

int ray_cylinder(t_ray *ray, t_cylinder *cylinder, t_hit *hit)
{
	double		t;
	int			top_cap_hit;
	int			bottom_cap_hit;
	int			cap_hit;
    t_vec3		oc;
	t_vec3		ray_axis_cross;
	t_vec3		oc_axis_cross;
	double		radius;
    float a;
    float b;
    float c;

	bottom_cap_hit = check_cap_hit(cylinder, *ray, hit, 0);
	top_cap_hit = check_cap_hit(cylinder, *ray, hit, cylinder->height);
	cap_hit = bottom_cap_hit || top_cap_hit;
	oc = vec3_sub(ray->origin, cylinder->center);
	radius = cylinder->diameter / 2.0;
	ray_axis_cross = vec3_cross(ray->direction, cylinder->axis);
	oc_axis_cross = vec3_cross(oc, cylinder->axis);
	a = vec3_dot(ray_axis_cross, ray_axis_cross);
	b = 2.0 * vec3_dot(ray_axis_cross, oc_axis_cross);
	c = vec3_dot(oc_axis_cross, oc_axis_cross) - radius * radius;
    float delta = b * b - 4.f * a * c;
	if (fabs(a) < 0.0001)
		return (cap_hit);
    float t1 = (-b - sqrtf(delta)) / (2.f * a);
    float t2 = (-b + sqrtf(delta)) / (2.f * a);
    t = fminf(t1, t2);
    if (t <= 0.001 || (cap_hit && t >= hit->t))
		return (cap_hit);
	if (!check_cylinder_surface(cylinder, *ray, hit, t))
		return (cap_hit);
	return (1);
}