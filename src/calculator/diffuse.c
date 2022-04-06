/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diffuse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 19:37:04 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/06 16:48:23 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <float.h>

bool	diffuse_next(t_thread *self, t_obj_l *objs, t_vector *start, t_vector *ray, t_vector *inter)
{
	bool	ret;

	ret = false;
	if (objs->obj_type == SPHERE)
		ret = hit_sphere(self, start, objs, ray, inter);
	else if (objs->obj_type == PLANE && fast_intersec_plane(ray, &objs->normal))
	{
		ret = intersec_plane(self, ray, start, objs, inter);
		//objs->disthit = vector_distance(start, inter);
	}
	//else if (objs->obj_type == LIGHT)
	//	ret = specular_highlight(start, objs, ray, inter);
	//else if (objs->obj_type == CYLINDER)
	//	return (calc_intersection_cylinder(mixer->cam, objs, ray))
	return (ret);
}

bool	diffuse_nearest(t_thread *self, t_diff *diff, t_vector *start, t_vector *result)
{
	t_vector	intersect;
	t_vector	new_intersect;
	t_obj_l		*objs;
	t_obj_l		*curr;
	float		distsf;
	bool		sw;

	objs = self->mixer->obj_list;
	sw = false;
	while (objs != NULL)
	{
		if (diff->hit != objs && sw == false && diffuse_next(self, objs, start, diff->ray, &intersect))
		{
			distsf = self->disthit;
			sw = true;
			curr = objs;
		}
		else if (diff->hit != objs && sw == true)
		{
			if (diffuse_next(self, objs, start, diff->ray, &new_intersect) && distsf > self->disthit)
			{
				intersect = new_intersect;
				distsf = self->disthit;
				curr = objs;
			}
		}
		objs = objs->next;
	}
	*result = intersect;
	diff->hit = curr;
	return (sw);
}

t_vector	diffuse_rand(t_thread *self, t_diff diff)
{
	t_vector	tmp;
	t_vector	reflection;
	float		inter;
	float x, y, z;
	
	inter = vector_scalar_product(diff.ray, &self->obj_int_normal);
	if (inter < 0)
		inter *= -1;
	vector_multiply_digit(&reflection, &self->obj_int_normal, inter * 2);
	vector_addition(&reflection, &reflection, diff.ray);
	x = (float) drand48() + reflection.x;
	y = (float) drand48() + reflection.y;
	z = (float) drand48() + reflection.z;
	tmp.x = (x - 0.5) * (diff.hit->diffusion * 0.5);
	tmp.y = (y - 0.5) * (diff.hit->diffusion * 0.5);
	tmp.z = (z - 0.5) * (diff.hit->diffusion * 0.5);
	vector_normalize(&tmp);
	vector_addition(&tmp, &tmp, &reflection);
	return (tmp);
}

//t_vector	diffuse_randi1() {
//	t_vector ret;
//	t_vector inter;
//
//	do {
//		vector_create(&inter, drand48(), drand48(), drand48());
//		vector_multiply_digit(&inter, &inter, 2);
//		vector_create(&ret, inter.x - 1, inter.y - 1, inter.z - 1);
//	} while ((ret.x * ret.x + ret.y * ret.y + ret.z * ret.z) >= 1);
//	return (ret);
//}

t_rgbof	vector_cast_rgbof(t_vector vec)
{
	t_rgbof	c;

	if (vec.x > 1)
		vec.x = 1;
	c.r = vec.x * 255;
	if (vec.y > 1)
		vec.y = 1;
	c.g = vec.y * 255;
	if (vec.z > 1)
		vec.z = 1;
	c.b = vec.z * 255;
	return (c);
}

t_vector	rgbof_cast_vector(t_rgbof color)
{
	t_vector vec;
	vec.x = (float) color.r / 255;
	vec.y = (float) color.g / 255;
	vec.z = (float) color.b / 255;
	return (vec);
}

t_vector	diffuse_get(t_thread *self, t_diff diff, t_vector *result)
{
	t_vector	inter;
	t_vector	inter2;
	t_vector	inter3;
	t_vector	ret;

	if (diff.ray_count < MAX_BOUNCES && diffuse_nearest(self, &diff, diff.origin, result))
	{
		*diff.ray = diffuse_rand(self, diff);
		vector_create(diff.origin, result->x, result->y, result->z);
		diff.ray_count += 1;
		inter2 = diffuse_get(self, diff, result);
		inter3 = rgbof_cast_vector(diff.hit->color);
		vector_multiply(&inter2, &inter2, &inter3);
		vector_multiply_digit(&inter, &inter2, self->mixer->ambient.a_light);
		return (inter);
	}
	else
	{
		ret = rgbof_cast_vector(self->mixer->ambient.color);
		return (ret);
	}
}

t_vector	diffuse_main(t_thread *self, t_obj_l *obj, t_vector *intersect)
{
	t_vector	ret;
	t_vector	res;
	t_vector	a;

	self->diff_sh.ray = intersect;
	self->diff_sh.hit = obj;
	self->diff_sh.ray_count = 0;
	self->diff_sh.origin = &a;
	vector_create(&a, self->mixer->cam.position.x, self->mixer->cam.position.y, self->mixer->cam.position.z);
	vector_create(&res, 0, 0, 0);
	ret = diffuse_get(self, self->diff_sh, &res);
	if (self->diff_sh.ray_count < 1)
		self->diff_sh.ray_count = 1;
	return (ret);
}
