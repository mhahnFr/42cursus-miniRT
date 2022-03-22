/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baseimage.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:23:59 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/22 22:39:54 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "vector.h"

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
			distsf = list->disthit;
			sw = true;
			curr = list;
		}
		else if (sw == true && intersec_next(list, origin, &ray, &intersect) && distsf > list->disthit)
		{
			distsf = list->disthit;
			curr = list;
		}
		list = list->next;
	}
	if (curr == NULL)
		return (false);
	intersect = rgbof_cast_vector(nointersec->color);
	inter = rgbof_cast_vector(light->color);
	vector_multiply(&intersect, &intersect, &inter);
	vector_multiply_digit(&intersect, &intersect, light->brightness);
	*color = vector_cast_rgbof(intersect);
	printf("%f %f %f\n", intersect.x, intersect.y, intersect.z);
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
			vector_addition(&ray, &intersect, &l->position);
			vector_normalize(&ray);
			if (intersect_object(mixer, curr, &intersect, l, ray, color))
				return (true);
		}
		l = l->next;
	}
	return (false);
}

t_rgbof	calc_intersect_vector(t_obj_l *nointersec, t_obj_l *list, t_vector *origin, t_vector *ray, t_mixer *mixer)
{
	t_vector	intersect;
	t_vector	intersect2;
	t_obj_l		*curr;
	t_rgbof		color;
	float		distsf;
	bool		sw;

	sw = false;
	curr = NULL;
	while (list != NULL)
	{
		if (nointersec != list  && sw == false && intersec_next(list, origin, ray, &intersect))
		{
			distsf = list->disthit;
			sw = true;
			curr = list;
			intersect2 = intersect;
		}
		else if (nointersec != list && sw == true && intersec_next(list, origin, ray, &intersect) && distsf > list->disthit)
		{
			distsf = list->disthit;
			curr = list;
			intersect2 = intersect;
		}
		list = list->next;
	}
	//uf (curr == NULL || trace_light(mixer, curr, &color, intersect2))
	color = diffuse_main(mixer, NULL, ray);
	//color = mixer->ambient.color;
	if (curr != NULL) trace_light(mixer, curr, &color, intersect2);
	//else color = diffuse_main(mixer, NULL, ray);
	//color.cal_r = 0;
	//color.cal_g = 0;
	//color.cal_b = 0;
	//color_rgb_cal_result_mul(&color, curr->color, 1);
	//color = color_cal_rgb(color, /*MAX_BOUNCES*/mixer->diff_sh.ray_count + 1);
	return (color);
}
