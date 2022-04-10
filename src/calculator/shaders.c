/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaders.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 18:26:15 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/10 17:29:50 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "vector.h"
#include <math.h>

t_vector	trace_next(t_mixer *mixer, t_vector intersect, t_vector ray, \
			t_obj_l *curr)
{
	ray = trace_rand(ray, curr->col_normal, curr->diffusion);
	return (rgbof_cast_vector(calc_shader(&intersect, &ray, mixer, \
											&mixer->col_sum)));
}

void	shader_reflection_shadow(t_mixer *mixer, t_obj_l *curr, t_col *colsum, \
	t_vector intersect2, t_vector *ray)
{
	t_vector	s_col;
	t_vector	l_col;
	t_vector	intersect;

	colsum->diff = rgbof_cast_vector(mixer->ambient.color);
	intersect = rgbof_cast_vector(curr->color);
	vector_create(&s_col, 1, 1, 1);
	vector_create(&l_col, 1, 1, 1);
	if (curr->reflec_fac > 0)
		s_col = trace_next(mixer, intersect2, *ray, curr);
	if (curr->reflec_fac < 1)
		l_col = trace_light(mixer, curr, intersect2);
	vector_multiply_digit(&s_col, &s_col, curr->reflec_fac);
	vector_multiply_digit(&l_col, &l_col, 1 - curr->reflec_fac);
	vector_addition(&intersect2, &s_col, &l_col);
	colsum->sum[0] = intersect2;
	colsum->fac[0] = 1;
	colsum->l_count = 1;
}

bool	check_hit(t_mixer *mixer, t_obj_l *curr, t_col *colsum, \
	t_vector intersect2, t_vector *ray)
{
	colsum->l_count = 0;
	if (curr == NULL)
		return (false);
	if (curr->obj_type == LIGHT)
	{
		colsum->diff = rgbof_cast_vector(curr->color);
		colsum->sum[0] = colsum->diff;
		colsum->fac[0] = 1;
		colsum->l_count = 1;
		return (false);
	}
	shader_reflection_shadow(mixer, curr, colsum, intersect2, ray);
	return (true);
}

bool	trace_hardshadow(t_mixer *mixer, t_col *colsum, t_vector *origin, \
		t_vector *ray)
{
	t_vector	intersect;
	t_vector	intersect2;
	t_obj_l		*curr;
	bool		sw;
	t_obj_l		*list;

	sw = false;
	curr = NULL;
	list = mixer->obj_list;
	while (list != NULL)
	{
		if (list->obj_type != LIGHT && intersec_next(list, origin, ray, &intersect) && (!sw || (sw \
			&& curr->disthit > list->disthit)))
		{
			sw = true;
			curr = list;
			intersect2 = intersect;
		}
		list = list->next;
	}
	return (check_hit(mixer, curr, colsum, intersect2, ray));
}

t_rgbof	calc_shader(t_vector *origin, t_vector *ray, t_mixer *mixer, \
		t_col *col_sum)
{
	t_rgbof		color;
	t_vector	cp;

	cp.x = ray->x;
	cp.y = ray->y;
	cp.z = ray->z;
	mixer->bounces++;
	if (mixer->bounces == MAX_BOUNCES + 1)
		return (mixer->ambient.color);
	color = color_rgb(mixer->ambient.color.r, mixer->ambient.color.g, \
	mixer->ambient.color.g);
	col_sum->diff = rgbof_cast_vector(color);
	if (trace_hardshadow(mixer, col_sum, origin, ray))
		col_sum->diff = diffuse_main(mixer, NULL, &cp);
	color = sumup_light(mixer, col_sum);
	return (color);
}
