/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diffuse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 19:37:04 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/18 22:30:05 by mhahn            ###   ########.fr       */
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

bool	diffuse_next(t_obj_l *objs, t_vector *start, t_vector *ray, t_vector	*inter)
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

bool	diffuse_nearest(t_mixer *mixer, t_vector *ray, t_obj_l *curr, t_vector *start, t_vector *result)
{
	t_vector	intersect;
	t_vector	new_intersect;
	t_obj_l		*objs;
	float		distsf;
	bool		sw;

	objs = mixer->obj_list;

	sw = false;
	while (objs != NULL)
	{
		if (curr != objs  && sw == false && diffuse_next(objs, start, ray, &intersect))
		{
			distsf = objs->disthit;
			sw = true;
		}
		else if (curr != objs && sw == true)
		{
			if (diffuse_next(objs, start, ray, &new_intersect) && distsf > objs->disthit)
			{
				intersect = new_intersect;
				distsf = objs->disthit;
			}
		}
		objs = objs->next;
	}
	*result = intersect;
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

t_rgbof	diffuse_main(t_mixer *mixer, t_obj_l *obj, t_vector *intersect)
{
	t_rgbof	color;
	color = diffuse_get(mixer, obj, *intersect);
	color.cal_r = color.r;
	color.cal_g = color.g;
	color.cal_b = color.b;
	return (color);
}

t_rgbof	diffuse_get(t_mixer *mixer, t_obj_l *obj, t_vector intersect)
{
	float	factor;
	t_rgbof	ret;
	t_vector	result;
	t_vector	inter;
	t_vector	inter2;
	t_vector	inter3;
	static int recc = 0;

	//factor = diffuse_nearest(mixer, &intersect, obj, &obj->col_normal);
	if (recc < MAX_BOUNCES && diffuse_nearest(mixer, &intersect, obj, &obj->col_normal, &result))
	{
		vector_addition(&inter, &result, &obj->col_normal);
		inter3 = diffuse_rand();
		vector_addition(&inter2, &inter, &inter3);
		vector_substract(&inter, &inter2, &result);
		return (color_cal_rgb(diffuse_get(mixer, obj, inter), 0.5f));
	}
	else
	{
		//float t = 0.5 * (obj->col_normal.y + 1.0f);
		recc = 0;
		return (mixer->ambient.color);
	}
	////if (obj->obj_type == PLANE)
	ret = obj->color;
	factor *= 0.5;
//	if (factor < 0.01)
//		factor = 1;
//	else if (factor < 0.1)
//		factor = 0.1;
//	else
//		factor = sqrtf(factor);
	if (factor > 1)
		factor = 1;
	//factor = 1;
	ret = color_rgb((int) (ret.r * factor), (int) (ret.g * factor), (int) (ret.b * factor));
	return (ret);
}

