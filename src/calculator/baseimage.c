/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baseimage.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:23:59 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/18 15:26:25 by jkasper          ###   ########.fr       */
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

t_rgbof	calc_intersect_vector(t_obj_l *nointersec, t_obj_l *list, t_vector *origin, t_vector *ray)
{
	t_vector	intersect;
	t_vector	new_intersect;
	t_rgbof		color;
	float		distsf;
	bool		sw;

	color = color_rgb(0,0,0);
	while (list != NULL)
	{
		if (nointersec != list  && sw == false && intersec_next(list, origin, ray, &intersect))
		{
			distsf = list->disthit;
			sw = true;
			color = list->color;
		}
		else if (nointersec != list && sw == true)
		{
			if (intersec_next(list, origin, ray, &new_intersect) && distsf > list->disthit)
			{
				intersect = new_intersect;
				distsf = list->disthit;
				color = list->color;
			}
		}
		list = list->next;
	}
	return (color);
}