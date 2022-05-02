/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_obj_intersection.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/24 16:23:59 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/25 14:20:34 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdio.h>
#include <math.h>

bool	intersec_next(
		t_obj_l *objs, t_vector *origin, t_vector *ray, t_vector *inter)
{
	bool	ret;

	ret = false;
	if (objs->obj_type == SPHERE)
		ret = hit_sphere(origin, objs, ray, inter);
	else if (objs->obj_type == PLANE && fast_intersec_plane(ray, &objs->normal))
		ret = intersec_plane(ray, origin, objs, inter);
	else if (objs->obj_type == CYLINDER)
		ret = hit_cylinder(origin, objs, ray, inter);
	return (ret);
}

/*
	Function does following black-magic, but in normed manner:

 	union
	{
		float f;
		uint32_t i;
	}
	conv = {.f = number};
	conv.i = 0x5f3759df - (conv.i >> 1);
	conv.f *= 1.5F - (number * 0.5F * conv.f * conv.f);
 */
float	light_distance_factor(float number)
{
	uint32_t	sec;
	float		result;
	void		*num_cp;

	num_cp = &number;
	sec = *(uint32_t *)num_cp;
	sec = 0x5f3759df - (sec >> 1);
	num_cp = &sec;
	result = (float)((1.5F - (number * 0.5F * \
	(*(float *) num_cp) * (*(float *) num_cp))) * (*(float *) num_cp));
	return (result);
}

void	intersect_obj_color(t_iobj *i_struc, float length)
{
	t_vector	intercol;
	float		fact;

	intercol = rgbof_cast_vector(i_struc->obj_col->color);
	i_struc->shadow = true;
	if (i_struc->curr == NULL)
	{
		fact = 0.5f;
		if (length > i_struc->light->intensity)
		{
			length = (length - i_struc->light->intensity);
			length /= i_struc->light->intensity / 2;
			fact += length;
		}
		fact = light_distance_factor(fact);
		i_struc->shadow = false;
		i_struc->ret_color = rgbof_cast_vector(i_struc->light->color);
		vector_multiply_digit(&i_struc->ret_color, &i_struc->ret_color, \
		i_struc->light->brightness * fact);
		vector_multiply(&i_struc->ret_color, &i_struc->ret_color, &intercol);
	}
	else
		i_struc->ret_color = intercol;
}

bool	intersect_object(
		t_mixer *mixer,
		t_iobj *i_struc,
		float length)
{
	float		distsf;
	bool		sw;

	sw = false;
	i_struc->curr = NULL;
	i_struc->list = mixer->obj_list;
	while (i_struc->list != NULL)
	{
		if (i_struc->list->obj_type != LIGHT && intersec_next(i_struc->list, \
		&i_struc->origin, &i_struc->ray, &i_struc->inter) && length > i_struc \
		->list->disthit && (!sw || distsf > i_struc->list->disthit))
		{
			distsf = i_struc->list->disthit;
			sw = true;
			i_struc->curr = i_struc->list;
		}
		i_struc->list = i_struc->list->next;
	}
	intersect_obj_color(i_struc, length);
	return (true);
}
