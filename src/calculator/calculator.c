/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculator.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:42:53 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/11 01:42:33 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minirt.h"
#include <math.h>
#include <stdbool.h>

void	calculator(t_mixer *mixer, int *ret)
{
	calc_object_ray(mixer, ret);
}

void	calc_object_ray(t_mixer *mixer, int *ret)
{
	(void) ret;
	for (int i = 0; i < RESOLUTION_Y; i++) {
		for (int j = 0; j < RESOLUTION_X; j++) {
			calc_intersec_first(mixer->obj_list, mixer, &(mixer->cam.vecs[i][j]));
		}
	}
}

void	skip_obj(t_obj_l **objs, int toskip)
{
	while ((*objs) != NULL && (*objs)->obj_type == toskip)
		(*objs) = (*objs)->next;
}


t_vector	calc_intersection_sphere(t_cam cam, t_obj_l *obj, t_vector *ray)
{
	float		B;
	float		C;
	float		t0;
	float		t1;
	t_vector	ret;
	
	B = ( ray->x * (cam.position.x - obj->position.x) + ray->y * (cam.position.y - obj->position.y) + ray->z * (cam.position.z - obj->position.z));
	C =  vector_scalar_product(&(cam.position), &(obj->position)) - powf(obj->height, 2);
	t0 = ((B) - sqrtf(powf(B, 2) - C));
	t1 = ((B) + sqrtf(powf(B, 2) - C));
	if (sqrtf(t0) < sqrtf(t1))
		t0 = t1;
	ret.x = cam.position.x + ray->x * t0;
	ret.y = cam.position.y + ray->y * t0;
	ret.z = cam.position.z + ray->z * t0;
	return (ret);
}

t_vector	calc_intersec_next(t_obj_l *objs, t_mixer *mixer, t_vector *ray)
{
	
	if (objs->obj_type == SPHERE && vector_scalar_product(&(mixer->cam.position), &(objs->position)) - powf(objs->height, 2) < 0)
		return (calc_intersection_sphere(mixer->cam, objs, ray));
	else if (objs->obj_type == PLANE && calc_intersecs_plane(ray, &objs->normal))
		return (calc_intersection_plane(ray, objs));
	else if (objs->obj_type == CYLINDER)
	{
		// TODO
	}	
}

bool	calc_intersec_dist(t_vector intersect, t_vector new_intersect, t_vector *cam)
{
	t_vector	inter;
	t_vector	inter2;

	vector_substract(&inter, &intersect, cam);
	vector_substract(&inter2, &new_intersect, cam);
	return (vector_length(&inter) > vector_length(&inter2));
}

void	calc_intersec_first(t_obj_l *objs, t_mixer *mixer, t_vector *ray)
{
	t_vector	intersect;
	t_vector	new_intersect;

	skip_obj(&objs, LIGHT);
	intersect = calc_intersec_next(objs, mixer, ray);
	objs = objs->next;
	while (objs != NULL)
	{
		new_intersect = calc_intersec_next(objs, mixer, ray);
		if (calc_intersec_dist(intersect, new_intersect, ray))
			intersect = new_intersect;
		objs = objs->next;
		skip_obj(&objs, LIGHT);
	}
}

t_vector	calc_intersection_plane(t_vector *cam, t_obj_l *objs)
{
	t_vector	vec2;
	t_vector	point;
	t_vector	vec_inter;
	float		d;

	vec2.x = cam->x;
	vec2.y = cam->y;
	vec2.z = cam->z;
	vector_normalize(&vec2);
	vector_multiply_digit(&vec_inter, &vec2, 0.5);
	vector_multiply(&vec2, &vec_inter, cam);
	vector_substract(&vec_inter, &(objs->position), &vec2);
	d = vector_scalar_product(&vec_inter, &(objs->normal)) / vector_scalar_product(cam, &(objs->normal));
	vector_multiply_digit(&vec_inter, cam, d);
	vector_addition(&point, &vec2, &vec_inter);
	return (point);
}

bool	calc_intersecs_plane(t_vector *vec, t_vector *normal)
{
	return (vector_scalar_product(vec, normal) != 0);
}
