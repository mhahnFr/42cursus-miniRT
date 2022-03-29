/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baseimage.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:23:59 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/29 12:06:09 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "vector.h"
#include <math.h>

t_vector	rgbof_light_strenght(t_vector *inter, t_vector *origin, t_obj_l *light, t_obj_l *ori, t_obj_l *curr)
{
	float	fact;

	*inter = rgbof_cast_vector(light->color);
	fact = 0.0f;
	vector_multiply_digit(inter, inter, fact);
	return (*inter);
}

bool	intersec_next(t_obj_l *objs, t_vector *origin, t_vector *ray, t_vector *inter)
{
	bool	ret;

	ret = false;
	if (objs->obj_type == SPHERE)
		ret = hit_sphere(origin, objs, ray, inter);
	else if (objs->obj_type == PLANE && fast_intersec_plane(ray, &objs->normal))
	{
		ret = intersec_plane(ray, objs, inter);
		objs->disthit = vector_distance(origin, inter);
	}
	//else if (objs->obj_type == CYLINDER)
	//	return (calc_intersection_cylinder(origin, objs, ray, inter))
	return (ret);
}

bool	intersect_object(t_mixer *mixer, t_obj_l *nointersec, t_vector *origin, t_obj_l *light, t_vector ray, t_rgbof *color)
{
	t_vector	intersect;
	t_vector	inter;
	t_vector	inter2;
	t_obj_l		*curr;
	t_obj_l		*list;
	float		distsf;
	bool		sw;

	sw = false;
	curr = NULL;
	list = mixer->obj_list;
	while (list != NULL)
	{
		if (sw == false && intersec_next(list, origin, &ray, &intersect))
		{
			inter = intersect;
			distsf = list->disthit;
			sw = true;
			curr = list;
		}
		else if (sw == true && intersec_next(list, origin, &ray, &intersect) && distsf > list->disthit)
		{
			inter = intersect;
			distsf = list->disthit;
			curr = list;
		}
		list = list->next;
	}
	intersect = rgbof_cast_vector(nointersec->color);
	//inter2 = rgbof_cast_vector(mixer->ambient.color);
	//vector_multiply(&intersect, &inter2, &intersect);
	//vector_multiply_digit(&intersect, &intersect, mixer->ambient.a_light);
	if (curr == NULL)
	{
		*color = vector_cast_rgbof(intersect);
		return (false);
	}
	inter = rgbof_light_strenght(&inter, origin, light, nointersec, curr);
	vector_multiply(&intersect, &intersect, &inter);
	*color = vector_cast_rgbof(intersect);
	return (true);
}

bool	trace_light(t_mixer *mixer, t_obj_l *curr, t_rgbof *color, t_vector intersect)
{
	t_vector	ray;
	t_obj_l		*l;

	l = mixer->obj_list;
	while (l != NULL)
	{
		if (l->obj_type == LIGHT)
		{
			vector_substract(&ray, &intersect, &l->position);
			vector_normalize(&ray);
			if (intersect_object(mixer, curr, &intersect, l, ray, color))
				return (true);
		}
		l = l->next;
	}
	return (false);
}

bool	trace_hardshadow(t_mixer *mixer, t_rgbof *color, t_vector *origin, t_vector *ray)
{
	t_vector	intersect;
	t_vector	intersect2;
	t_obj_l		*curr;
	float		distsf;
	bool		sw;
	t_obj_l		*list;

	sw = false;
	curr = NULL;
	list = mixer->obj_list;
	while (list != NULL)
	{
		if (sw == false && intersec_next(list, origin, ray, &intersect))
		{
			distsf = list->disthit;
			sw = true;
			curr = list;
			intersect2 = intersect;
		}
		else if (sw == true && intersec_next(list, origin, ray, &intersect) && distsf > list->disthit)
		{
			distsf = list->disthit;
			curr = list;
			intersect2 = intersect;
		}
		list = list->next;
	}
	if (curr != NULL)
		trace_light(mixer, curr, color, intersect2);
	return (curr != NULL);
}

t_rgbof	calc_shader(t_vector *origin, t_vector *ray, t_mixer *mixer)
{
	t_rgbof		color;
	t_vector	cp;

	cp.x = ray->x;
	cp.y = ray->y;
	cp.z = ray->z;
	color = color_cal_rgb(mixer->ambient.color, mixer->ambient.a_light);
	trace_hardshadow(mixer, &color, origin, ray);
	if (color.r == color.g && color.g == color.b && color.b == 0)
		color = diffuse_main(mixer, NULL, &cp);
	return (color);
}
