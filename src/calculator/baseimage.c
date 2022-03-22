/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baseimage.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:23:59 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/22 20:19:57 by mhahn            ###   ########.fr       */
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

t_rgbof	calc_intersect_vector(t_obj_l *nointersec, t_obj_l *list, t_vector *origin, t_vector *ray, t_mixer *mixer)
{
	t_vector	intersect;
	t_obj_l		*curr;
	t_rgbof		color;
	float		distsf;
	bool		sw;

	sw = false;
	curr = NULL;

	//color = color_rgb(255, 255, 255);
/*	while (list != NULL)
	{
		if (nointersec != list  && sw == false && intersec_next(list, origin, ray, &intersect))
		{
			distsf = list->disthit;
			sw = true;
			curr = list;
		}
		else if (nointersec != list && sw == true && intersec_next(list, origin, ray, &intersect) && distsf > list->disthit)
		{
			distsf = list->disthit;
			curr = list;
		}
		list = list->next;
	}
	color = mixer->ambient.color;
	if (curr == NULL)
		return (color);*/
	color = diffuse_main(mixer, NULL, ray);
	//color.cal_r = 0;
	//color.cal_g = 0;
	//color.cal_b = 0;
	//color_rgb_cal_result_mul(&color, curr->color, 1);
	//color = color_cal_rgb(color, /*MAX_BOUNCES*/mixer->diff_sh.ray_count + 1);
	return (color);
}
