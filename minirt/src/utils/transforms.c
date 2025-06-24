#include "../../includes/scene_math.h"
#include <stdio.h>

/*
** Create identity transform
*/
t_transform	transform_identity(void)
{
	t_transform	transform;

	transform.translation = vec3_create(0, 0, 0);
	transform.rotation = vec3_create(0, 0, 0);
	transform.scale = vec3_create(1, 1, 1);
	transform.matrix = matrix4_identity();
	return (transform);
}

/*
** Update transformation matrix from translation, rotation, scale
*/
void	transform_update_matrix(t_transform *transform)
{
	t_matrix4	temp;
	t_matrix4	rotation_combined;

	temp = matrix4_scale(transform->scale);
	rotation_combined = matrix4_multiply(matrix4_rotation_x(transform->rotation.x),
			temp);
	rotation_combined = matrix4_multiply(matrix4_rotation_y(transform->rotation.y),
			rotation_combined);
	rotation_combined = matrix4_multiply(matrix4_rotation_z(transform->rotation.z),
			rotation_combined);
	transform->matrix = matrix4_multiply(matrix4_translation(transform->translation),
			rotation_combined);
}

/*
** Add translation to transform
*/
void	transform_translate(t_transform *transform, t_vec3 translation)
{
	transform->translation = vec3_add(transform->translation, translation);
	transform_update_matrix(transform);
}

/*
** Add rotation to transform
*/
void	transform_rotate(t_transform *transform, t_vec3 rotation)
{
	transform->rotation = vec3_add(transform->rotation, rotation);
	transform_update_matrix(transform);
}

/*
** Apply uniform scale to transform
*/
void	transform_scale_uniform(t_transform *transform, double scale)
{
	transform->scale = vec3_mult(transform->scale, scale);
	transform_update_matrix(transform);
}

/*
** Apply non-uniform scale to transform
*/
void	transform_scale(t_transform *transform, t_vec3 scale)
{
	transform->scale.x *= scale.x;
	transform->scale.y *= scale.y;
	transform->scale.z *= scale.z;
	transform_update_matrix(transform);
}

/*
** Transform a sphere
*/
void	transform_sphere(t_sphere *sphere, t_transform *transform)
{
	sphere->center = matrix4_transform_point(transform->matrix, sphere->center);
	/* ** For sphere diameter, use uniform scale factor ** */
	if (transform->scale.x == transform->scale.y
		&& transform->scale.y == transform->scale.z)
		sphere->diameter *= transform->scale.x;
}

/*
** Transform a plane
*/
void	transform_plane(t_plane *plane, t_transform *transform)
{
	plane->point = matrix4_transform_point(transform->matrix, plane->point);
	plane->normal = matrix4_transform_direction(transform->matrix,
			plane->normal);
}

/*
** Transform a cylinder
*/
void	transform_cylinder(t_cylinder *cylinder, t_transform *transform)
{
	cylinder->center = matrix4_transform_point(transform->matrix,
			cylinder->center);
	cylinder->axis = matrix4_transform_direction(transform->matrix,
			cylinder->axis);
	/* ** For cylinder diameter, use uniform scale factor ** */
	if (transform->scale.x == transform->scale.y
		&& transform->scale.y == transform->scale.z)
	{
		cylinder->diameter *= transform->scale.x;
		cylinder->height *= transform->scale.y;
	}
}

/*
** Transform a cone
*/
void	transform_cone(t_cone *cone, t_transform *transform)
{
	cone->vertex = matrix4_transform_point(transform->matrix, cone->vertex);
	cone->axis = matrix4_transform_direction(transform->matrix, cone->axis);
	/* ** For cone height, use uniform scale factor ** */
	if (transform->scale.x == transform->scale.y
		&& transform->scale.y == transform->scale.z)
		cone->height *= transform->scale.y;
}

/*
** Transform a camera
*/
void	transform_camera(t_camera *camera, t_transform *transform)
{
	camera->position = matrix4_transform_point(transform->matrix,
			camera->position);
	camera->orientation = matrix4_transform_direction(transform->matrix,
			camera->orientation);
}

/*
** Translate object in scene
*/
void	scene_translate_object(t_scene *scene, int obj_index, t_vec3 delta)
{
	t_transform	transform;

	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;
	transform = transform_identity();
	transform_translate(&transform, delta);
	if (scene->objects[obj_index].type == SPHERE)
		transform_sphere(&scene->objects[obj_index].data.sphere, &transform);
	else if (scene->objects[obj_index].type == PLANE)
		transform_plane(&scene->objects[obj_index].data.plane, &transform);
	else if (scene->objects[obj_index].type == CYLINDER)
		transform_cylinder(&scene->objects[obj_index].data.cylinder,
			&transform);
	else if (scene->objects[obj_index].type == CONE)
		transform_cone(&scene->objects[obj_index].data.cone, &transform);
}

/*
** Rotate object in scene around its own center
*/
void	scene_rotate_object(t_scene *scene, int obj_index, t_vec3 rotation)
{
	t_vec3	axis;
	double	angle;

	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;
	/* Convert rotation vector to axis and angle */
	angle = vec3_length(rotation);
	if (angle < 0.0001) // Too small to rotate
		return ;
	axis = vec3_normalize(rotation);
	/* Apply rotation based on object type */
	if (scene->objects[obj_index].type == SPHERE)
	{
		/* Spheres don't need rotation as they look the same from all angles */
		return ;
	}
	else if (scene->objects[obj_index].type == PLANE)
	{
		/* Rotate plane normal */
		scene->objects[obj_index].data.plane.normal = vec3_rotate_around_axis(scene->objects[obj_index].data.plane.normal,
				axis, angle);
		scene->objects[obj_index].data.plane.normal = vec3_normalize(scene->objects[obj_index].data.plane.normal);
	}
	else if (scene->objects[obj_index].type == CYLINDER)
	{
		/* Rotate cylinder axis */
		scene->objects[obj_index].data.cylinder.axis = vec3_rotate_around_axis(scene->objects[obj_index].data.cylinder.axis,
				axis, angle);
		scene->objects[obj_index].data.cylinder.axis = vec3_normalize(scene->objects[obj_index].data.cylinder.axis);
	}
	else if (scene->objects[obj_index].type == CONE)
	{
		/* Rotate cone axis */
		scene->objects[obj_index].data.cone.axis = vec3_rotate_around_axis(scene->objects[obj_index].data.cone.axis,
				axis, angle);
		scene->objects[obj_index].data.cone.axis = vec3_normalize(scene->objects[obj_index].data.cone.axis);
	}
}

/*
** Scale object in scene
*/
void	scene_scale_object(t_scene *scene, int obj_index, double scale)
{
	t_transform	transform;

	if (obj_index < 0 || obj_index >= scene->num_objects)
		return ;
	transform = transform_identity();
	transform_scale_uniform(&transform, scale);
	if (scene->objects[obj_index].type == SPHERE)
		transform_sphere(&scene->objects[obj_index].data.sphere, &transform);
	else if (scene->objects[obj_index].type == CYLINDER)
		transform_cylinder(&scene->objects[obj_index].data.cylinder,
			&transform);
	else if (scene->objects[obj_index].type == CONE)
		transform_cone(&scene->objects[obj_index].data.cone, &transform);
}

/*
** Translate camera in scene
*/
void	scene_translate_camera(t_scene *scene, t_vec3 delta)
{
	t_transform	transform;

	transform = transform_identity();
	transform_translate(&transform, delta);
	transform_camera(&scene->camera, &transform);
}

/*
** Rotate camera in scene
*/
void	scene_rotate_camera(t_scene *scene, t_vec3 rotation)
{
	t_transform	transform;

	transform = transform_identity();
	transform_rotate(&transform, rotation);
	transform_camera(&scene->camera, &transform);
}
