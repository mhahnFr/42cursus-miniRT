/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baseimage.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:23:59 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/20 14:20:34 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "vector.h"
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
	//else if (objs->obj_type == LIGHT)
	//	ret = specular_highlight(origin, objs, ray, inter);
	//else if (objs->obj_type == CYLINDER)
	//	return (hit_cylinder(origin, objs, ray, inter))
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
	uint32_t sec;
	float	 result;
	void	 *num_cp;

	num_cp = &number;
	sec = *(uint32_t *)num_cp;
	sec = 0x5f3759df - (sec >> 1);
	num_cp = &sec;
	result = (float) ((1.5F - (number * 0.5F * \
	(*(float *) num_cp) * (*(float *) num_cp))) * (*(float *) num_cp));
	return (result);
}

bool	intersect_object(
			t_mixer *mixer,
			t_obj_l *nointersec,
			t_vector *origin,
			t_obj_l *light,
			t_vector ray,
			t_vector *color,
			float length)
{
	t_vector	a;
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
		if (!sw && list->obj_type
			!= LIGHT && intersec_next(list, origin, &ray, &intersect))
		{
			inter = intersect;
			distsf = list->disthit;
			sw = true;
			curr = list;
		}
		else if (sw && list->obj_type != LIGHT
			&& intersec_next(list, origin, &ray, &intersect)
			&& distsf > list->disthit)
		{
			inter = intersect;
			distsf = list->disthit;
			curr = list;
		}
		list = list->next;
	}
	*color = rgbof_cast_vector(light->color);
	a = rgbof_cast_vector(nointersec->color);
	vector_multiply(color, color, &a);
	if (curr == NULL)
		vector_multiply_digit(color, color, light->brightness
			* light_distance_factor(length * 0.5));
	else
		vector_multiply_digit(color, color, light->brightness / 10);
	return (true);
}

t_rgbof	sumup_light(t_mixer *mixer, t_col *c_s)
{
	t_rgbof		color;
	t_vector	final_color;
	t_vector	inter;
	float		fac_sum;
	int			i;

	vector_create(&final_color, 0, 0, 0);
	fac_sum = 1.0f / (mixer->light_count);
	i = 0;
	while (i < c_s->l_count)
	{
		vector_multiply_digit(&c_s->sum[i], &c_s->sum[i], c_s->fac[i]);
		vector_addition(&final_color, &final_color, &(c_s->sum[i]));
		i++;
	}
	if (i == 0)
		final_color = c_s->diff;
	else
	{
		vector_multiply_digit(&inter, &c_s->diff, mixer->ambient.a_light);
		vector_addition(&final_color, &final_color, &inter);
	}
	color = vector_cast_rgbof(final_color);
	return (color);
}

t_vector	trace_light(t_mixer *mixer, t_obj_l *curr, t_vector intersect)
{
	t_vector	ray;
	t_vector	added;
	t_vector	sum;
	float		length;
	t_obj_l		*l;

	vector_create(&sum, 0, 0, 0);
	l = mixer->obj_list;
	while (l != NULL)
	{
		if (l->obj_type == LIGHT)
		{
			vector_substract(&ray, &l->position, &intersect);
			length = vector_length(&ray);
			vector_normalize(&ray);
			intersect_object(mixer, curr, &intersect, l, ray, &added, length);
			vector_addition(&sum, &sum, &added);
		}
		l = l->next;
	}
	return (sum);
}

t_vector	trace_rand(t_vector ray, t_vector normal, float diffusion)
{
	t_vector	tmp;
	t_vector	reflection;
	float		inter;
	float		rand_val;

	inter = vector_scalar_product(&ray, &normal);
	if (inter < 0)
		inter *= -1;
	vector_multiply_digit(&reflection, &normal, inter * 2);
	vector_addition(&reflection, &reflection, &ray);
	if (diffusion == 0)
		return (reflection);
	rand_val = (float) drand48();
	tmp.x = ((rand_val - 0.5) * diffusion) + reflection.x;
	rand_val = (float) drand48();
	tmp.y = ((rand_val - 0.5) * diffusion) + reflection.y;
	rand_val = (float) drand48();
	tmp.z = ((rand_val - 0.5) * diffusion) + reflection.z;
	vector_normalize(&tmp);
	vector_addition(&tmp, &tmp, &reflection);
	return (tmp);
}
