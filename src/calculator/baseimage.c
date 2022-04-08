/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baseimage.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:23:59 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/08 18:28:45 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "vector.h"
#include <math.h>

bool	intersec_next(t_obj_l *objs, t_vector *origin, t_vector *ray, \
		t_vector *inter)
{
	bool	ret;

	ret = false;
	if (objs->obj_type == SPHERE)
		ret = hit_sphere(origin, objs, ray, inter);
	else if (objs->obj_type == PLANE && fast_intersec_plane(ray, &objs->normal))
		ret = intersec_plane(ray, origin, objs, inter);
	else if (objs->obj_type == LIGHT)
		ret = specular_highlight(origin, objs, ray, inter);
	//else if (objs->obj_type == CYLINDER)
	//	return (calc_intersection_cylinder(origin, objs, ray, inter))
	return (ret);
}

bool	intersect_object(t_mixer *mixer, t_obj_l *nointersec, t_vector *origin, \
t_obj_l *light, t_vector ray, t_vector *color)
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
		if (sw == false && list->obj_type != LIGHT && intersec_next(list, origin, &ray, &intersect))
		{
			inter = intersect;
			distsf = list->disthit;
			sw = true;
			curr = list;
		}
		else if (sw == true && list->obj_type != LIGHT && intersec_next(list, origin, &ray, &intersect) && distsf > list->disthit)
		{
			inter = intersect;
			distsf = list->disthit;
			curr = list;
		}
		list = list->next;
	}
	*color = rgbof_cast_vector(light->color);
	t_vector a = rgbof_cast_vector(nointersec->color);
	vector_multiply(color, color, &a);
	if (curr == NULL)
		vector_multiply_digit(color, color, light->brightness);
	else
		vector_multiply_digit(color, color, 0.1);
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

float	light_distance_factor(float length, float brightness, float intensity)
{
	int		inter;
	float	inter2;
	int		i;

	i = 0;
	inter2 = length / intensity;
	if (length < intensity)
		return (brightness - (brightness * 0.5f * inter2));
	inter = length / intensity;
	inter2 /= powf(inter, 2.0f);
	length = length - inter;
	inter2 = inter2 - ((float) length / intensity) * 0.5f;
	if (inter2 < 0.1f)
		return (0.1f);
	return (inter2);
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
			intersect_object(mixer, curr, &intersect, l, ray, &added);
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
