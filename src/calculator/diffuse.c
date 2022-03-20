/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diffuse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 19:37:04 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/20 19:59:21 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

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
		ret = intersec_plane(ray, objs, inter);
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
		if (diff->hit != objs  && sw == false && diffuse_next(diff->hit, start, diff->ray, &intersect))
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
	
	x = (float) (rand() % 100) / 100;
	y = (float) (rand() % 100) / 100;
	z = (float) (rand() % 100) / 100;
	tmp.x += x;
	tmp.y += y;
	tmp.z += z;
	vector_normalize(&tmp);
	return (tmp);
}

/*t_rgbof	diffuse_main(t_mixer *mixer, t_obj_l *obj, t_vector *intersect)
{
	int		i;
	t_rgbof	color;
	t_rgbof	add;

	i = DIFFUSE;
	vector_multiply_digit(&obj->col_normal, &obj->col_normal, obj->height);
	color = obj->color;
	//color = diffuse_get(mixer, obj, intersect);
	color.cal_r = color.r;
	color.cal_g = color.g;
	color.cal_b = color.b;
	while (i > 0)
	{
		add = diffuse_get(mixer, obj, diffuse_rand(*intersect));
		color = color_add_cal(color, add);
		i--;
	}
	return (color_cal_rgb(color, DIFFUSE));
}*/

t_rgbof	diffuse_get(t_mixer *mixer, t_diff diff)
{
	t_vector	inter;
	t_vector	result;
	t_vector	inter2;
	t_vector	inter3;

	if (diff.ray_count < MAX_BOUNCES && diffuse_nearest(mixer, &diff, diff.origin, &result))
	{
		vector_addition(&inter, &result, diff.origin);
		inter3 = diffuse_rand();
		vector_addition(&inter2, &inter, &inter3);
		vector_substract(&inter, &inter2, &result);
		diff.ray_count++;
		vector_addition(diff.origin, &diff.hit->position, &diff.hit->col_normal);
		diff.ray = &inter;
		return (color_cal_rgb(diffuse_get(mixer, diff), 0.5f));
	}
	else
	{
		//float t = 0.5 * (obj->col_normal.y + 1.0f);
		diff.ray_count = 0;
		return (mixer->ambient.color);
	}
}

t_rgbof	diffuse_main(t_mixer *mixer, t_obj_l *obj, t_vector *intersect)
{
	t_rgbof	color;
	mixer->diff_sh.ray = intersect;
	mixer->diff_sh.hit = obj;
	mixer->diff_sh.ray_count = 0;
	vector_addition(mixer->diff_sh.origin, &obj->col_normal, &obj->position);
	color = diffuse_get(mixer, mixer->diff_sh);
	color.cal_r = color.r;
	color.cal_g = color.g;
	color.cal_b = color.b;
	return (color);
}
