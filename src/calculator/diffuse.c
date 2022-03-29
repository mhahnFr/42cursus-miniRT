/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diffuse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 19:37:04 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/29 16:56:37 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <float.h>

/* bool	calc_intersection_plane(t_vector *cam, t_obj_l *objs, t_vector *point)
{
	t_vector	vec2;
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
	if (d < 0)
		return (false);
	vector_multiply_digit(&vec_inter, cam, d);
	vector_addition(point, &vec2, &vec_inter);
	objs->disthit = d;
	objs->col_normal = objs->normal;
	return (true);
} */

bool	diffuse_next(t_obj_l *objs, t_vector *start, t_vector *ray, t_vector *inter)
{
	bool	ret;

	ret = false;
	if (objs->obj_type == SPHERE)
		ret = hit_sphere(start, objs, ray, inter);
	else if (objs->obj_type == PLANE && fast_intersec_plane(ray, &objs->normal))
	{
		ret = intersec_plane(ray, start, objs, inter);
		//objs->disthit = vector_distance(start, inter);
	}
	//else if (objs->obj_type == CYLINDER)
	//	return (calc_intersection_cylinder(mixer->cam, objs, ray))
	return (ret);
}

bool	diffuse_nearest(t_mixer *mixer, t_diff *diff, t_vector *start, t_vector *result)
{
	t_vector	intersect;
	t_vector	new_intersect;
	t_obj_l		*objs;
	t_obj_l		*curr;
	float		distsf;
	bool		sw;

	objs = mixer->obj_list;

	sw = false;
	while (objs != NULL)
	{
		if (diff->hit != objs  && sw == false && diffuse_next(objs, start, diff->ray, &intersect))
		{
			distsf = objs->disthit;
			sw = true;
			curr = objs;
		}
		else if (diff->hit != objs && sw == true)
		{
			if (diffuse_next(objs, start, diff->ray, &new_intersect) && distsf > objs->disthit)
			{
				intersect = new_intersect;
				distsf = objs->disthit;
				curr = objs;
			}
		}
		objs = objs->next;
	}
	*result = intersect;
	diff->hit = curr;
	return (sw);
}

t_vector	diffuse_rand()
{
	t_vector	tmp;
	float x, y, z;
	
	x = (float) (arc4random() % 100) / 100;
	y = (float) (arc4random() % 100) / 100;
	z = (float) (arc4random() % 100) / 100;
	tmp.x = x - 0.5;
	tmp.y = y - 0.5;
	tmp.z = z - 0.5;
	vector_normalize(&tmp);
	return (tmp);
}

t_vector	diffuse_randi1() {
	t_vector ret;
	t_vector inter;

	do {
		vector_create(&inter, drand48(), drand48(), drand48());
		vector_multiply_digit(&inter, &inter, 2);
		vector_create(&ret, inter.x - 1, inter.y - 1, inter.z - 1);
	} while ((ret.x * ret.x + ret.y * ret.y + ret.z * ret.z) >= 1);
	return (ret);
}

t_rgbof	vector_cast_rgbof(t_vector vec)
{
	t_rgbof	c;

	c.r = vec.x * 255;
	c.g = vec.y * 255;
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

t_vector	diffuse_get(t_mixer *mixer, t_diff diff, t_vector *result)
{
	t_vector	inter;
	t_vector	inter2;
	t_vector	inter3;

	if (diff.ray_count < MAX_BOUNCES && diffuse_nearest(mixer, &diff, diff.origin, result))
	{
		vector_addition(&inter, result, &diff.hit->col_normal);
		inter2 = diffuse_rand();
		vector_addition(&inter3, &inter, &inter2); // = target
		vector_substract(diff.ray, &inter3, result);
		diff.origin = vector_new(result->x, result->y, result->z);
		diff.ray_count += 1;
		inter2 = diffuse_get(mixer, diff, result);
		inter3 = rgbof_cast_vector(diff.hit->color);
		vector_multiply(&inter2, &inter2, &inter3);
		vector_multiply_digit(&inter, &inter2, mixer->ambient.a_light);
		return (inter);
	}
	else
		return (rgbof_cast_vector(mixer->ambient.color));
}

t_rgbof	diffuse_main(t_mixer *mixer, t_obj_l *obj, t_vector *intersect)
{
	t_rgbof	color;
	mixer->diff_sh.ray = intersect;
	mixer->diff_sh.hit = obj;
	mixer->diff_sh.ray_count = 0;
	mixer->diff_sh.origin = vector_new(mixer->cam.position.x, mixer->cam.position.y, mixer->cam.position.z);
	//mixer->diff_sh.origin = vector_new(obj->col_normal.x, obj->col_normal.y, obj->col_normal.z);
	//vector_addition(mixer->diff_sh.origin, &obj->col_normal, &obj->position);
	t_vector* res = vector_new(0, 0, 0);
	color = vector_cast_rgbof(diffuse_get(mixer, mixer->diff_sh, res));
	color.cal_r = color.r;
	color.cal_g = color.g;
	color.cal_b = color.b;
	if (mixer->diff_sh.ray_count < 1)
		mixer->diff_sh.ray_count = 1;
	return (color);
}
