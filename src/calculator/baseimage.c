/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baseimage.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:23:59 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/18 22:13:38 by mhahn            ###   ########.fr       */
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
	while (list != NULL)
	{
		if (nointersec != list  && sw == false && intersec_next(list, origin, ray, &intersect))
		{
			distsf = list->disthit;
			sw = true;
			curr = list;
//			color = list->color;
		}
		else if (nointersec != list && sw == true && intersec_next(list, origin, ray, &intersect) && distsf > list->disthit)
		{
				//intersect = new_intersect;
			distsf = list->disthit;
			curr = list;
//				color = diffuse_main(mixer, list, &intersect);
				//color = list->color;
		}
		list = list->next;
	}
	color = color_cal_rgb(mixer->ambient.color, mixer->ambient.a_light);
	if (curr == NULL)
		return (color);
	color = diffuse_main(mixer, curr, &intersect);
	color.cal_r += curr->color.r;
	color.cal_g += curr->color.g;
	color.cal_b += curr->color.b;
	color_cal_rgb(color, 2);
	return (color);
}
