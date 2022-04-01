/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baseimage.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:23:59 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/01 19:13:44 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "vector.h"
#include <math.h>

//t_vector	rgbof_light_strenght(t_vector *inter, t_vector *origin, t_obj_l *light, t_obj_l *ori)
//{
//	t_vector	a;
//
//	// TODO Calculate lightloss along distance
//	if (light == NULL)
//		return (rgbof_cast_vector(ori->color));
//	*inter = rgbof_cast_vector(light->color);
//	a = rgbof_cast_vector(ori->color);
//	vector_multiply(inter, inter, &a);
//	//vector_multiply_digit(inter, &b, light->brightness);
//	return (*inter);
//}

bool	intersec_next(t_obj_l *objs, t_vector *origin, t_vector *ray, t_vector *inter)
{
	bool	ret;

	ret = false;
	if (objs->obj_type == SPHERE)
		ret = hit_sphere(origin, objs, ray, inter);
	else if (objs->obj_type == PLANE && fast_intersec_plane(ray, &objs->normal))
	{
		ret = intersec_plane(ray, origin, objs, inter);
		objs->disthit = vector_distance(origin, inter);
	}
	//else if (objs->obj_type == CYLINDER)
	//	return (calc_intersection_cylinder(origin, objs, ray, inter))
	return (ret);
}

bool	intersect_object(t_mixer *mixer, t_obj_l *nointersec, t_vector *origin, t_obj_l *light, t_vector ray, t_vector *color)
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
	//t_vector col = rgbof_cast_vector(nointersec->color);
	//t_vector amb = rgbof_cast_vector(mixer->ambient.color);
	//vector_multiply(&inter, &col, &amb);
	//vector_multiply_digit(&col, &inter, mixer->ambient.a_light);
	*color = rgbof_cast_vector(light->color);
	t_vector a = rgbof_cast_vector(nointersec->color);
	vector_multiply(color, color, &a);
	if (curr == NULL)
	{
		vector_multiply_digit(color, color, light->brightness);
		return (false);
	}
	vector_multiply_digit(color, color, 0.1);
	return (false);
}

t_rgbof	sumup_light(t_mixer *mixer, t_col *col_sum)
{
	t_rgbof		color;
	t_vector	final_color;
	t_vector	inter;
	float		fac_sum;
	int	i;
	i = 0;
	vector_create(&final_color, 0, 0, 0);;
	fac_sum = 1.0f / (mixer->light_count);
	i = 0;
	while (i < col_sum->l_count)
	{
		vector_multiply_digit(&(col_sum->sum[i]), &(col_sum->sum[i]), (col_sum->fac[i]/* * fac_sum*/));
		vector_addition(&final_color, &final_color, &(col_sum->sum[i]));
		i++;
	}
	if (i == 0)
		final_color = col_sum->diff;
	else
	{
		vector_multiply_digit(&inter, &col_sum->diff, mixer->ambient.a_light/* * fac_sum*/);
		vector_addition(&final_color, &final_color, &inter);
	}
	//vector_multiply_digit(&final_color, &final_color, fac_sum);
	color = vector_cast_rgbof(final_color);
	return (color);
}

float	light_distance_factor(float length, float brightness, float intensity)
{
	int		inter;
	float	inter2;
	int	i;

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

t_vector	trace_light(t_mixer *mixer, t_obj_l *curr, t_col *col_sum, t_vector intersect)
{
	t_vector	inter;
	t_vector	ray;
	t_vector	added;
	t_vector	sum;
	float		length;
	t_obj_l		*l;
	bool		ret;
	
	ret = true;
	vector_create(&sum, 0,0,0);
	col_sum->l_count = 0;
	l = mixer->obj_list;
	col_sum->basecolor = rgbof_cast_vector(curr->color);
	while (l != NULL)
	{
		if (l->obj_type == LIGHT)
		{
			vector_substract(&ray, &l->position, &intersect);
			length = vector_length(&ray);
			vector_normalize(&ray);
			if (!intersect_object(mixer, curr, &intersect, l, ray, &added))
			{
				//length = light_distance_factor(length, l->brightness, l->intensity);
				vector_addition(&sum, &sum, &added);
				////col_sum->sum[col_sum->l_count] = rgbof_cast_vector(l->color);
				////length = l->brightness;
				//if (length != 0)
				//{
				//	col_sum->fac[col_sum->l_count++] = length;
				//	if (col_sum->l_count == 1)
				//		ret = false;
				//	//if (length < l->brightness)
				//	//	ret = true;
				//}
			}
		}
		l = l->next;
	}
	return (sum);
	//return (rgbof_cast_vector(curr->color));
}

t_vector	trace_rand(t_vector ray, t_vector normal, float diffusion)
{
	t_vector	tmp;
	t_vector	reflection;
	float		inter;
	float x, y, z;
	
	inter = vector_scalar_product(&ray, &normal);
	if (inter < 0)
		inter *= -1;
	vector_multiply_digit(&reflection, &normal, inter * 2);
	vector_addition(&reflection, &reflection, &ray);
	if (diffusion == 0)
		return (reflection);
	x = (float) drand48();
	y = (float) drand48();
	z = (float) drand48();
	tmp.x = ((x - 0.5) * diffusion) + reflection.x;
	tmp.y = ((y - 0.5) * diffusion) + reflection.y;
	tmp.z = ((z - 0.5) * diffusion) + reflection.z;
	vector_normalize(&tmp);
	vector_addition(&tmp, &tmp, &reflection);
	return (tmp);
}

t_vector	trace_next(t_mixer *mixer, t_vector intersect, t_vector ray, t_obj_l *curr)
{
	t_vector	inter;
	t_vector	inter2;

	ray = trace_rand(ray, curr->col_normal, curr->diffusion);
	return (rgbof_cast_vector(calc_shader(&intersect, &ray, mixer, &mixer->col_sum)));
}

bool	trace_hardshadow(t_mixer *mixer, t_col *colsum, t_vector *origin, t_vector *ray)
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
	colsum->sw = false;
	if (curr != NULL)
	{
		//printf("%f %f\n", curr->diffusion, curr->reflec_fac);
		colsum->diff = rgbof_cast_vector(mixer->ambient.color);
		intersect = rgbof_cast_vector(curr->color);
		//vector_multiply(&colsum->diff, &colsum->diff, &intersect);
		t_vector s_col, l_col;
		vector_create(&s_col, 1, 1, 1);
		vector_create(&l_col, 1, 1, 1);
		if (curr->reflec_fac > 0)
			s_col = trace_next(mixer, intersect2, *ray, curr);
		if (curr->reflec_fac < 1)
			l_col = trace_light(mixer, curr, colsum, intersect2);
		vector_multiply_digit(&s_col, &s_col, curr->reflec_fac);
		vector_multiply_digit(&l_col, &l_col, 1 - curr->reflec_fac);
		vector_addition(&intersect2, &s_col, &l_col);
		colsum->sum[0] = intersect2;
		colsum->fac[0] = 1;
		colsum->l_count = 1;
		return (true);
	}
	colsum->l_count = 0;
	return (false);
}

t_rgbof	calc_shader(t_vector *origin, t_vector *ray, t_mixer *mixer, t_col *col_sum)
{
	static int	bounces = 0;
	t_rgbof		color;
	t_vector	cp;

	cp.x = ray->x;
	cp.y = ray->y;
	cp.z = ray->z;
	bounces++;
	if (bounces == MAX_BOUNCES + 1)
		return (mixer->ambient.color);
	col_sum->sw = false;
	//if (bounces == 1)
		color = color_rgb(mixer->ambient.color.r , mixer->ambient.color.g, mixer->ambient.color.g);
	//else
	//	color = color_rgb(mixer->ambient.color.r * mixer->ambient.a_light, mixer->ambient.color.g * mixer->ambient.a_light, mixer->ambient.color.g * mixer->ambient.a_light);
	col_sum->diff = rgbof_cast_vector(color);
	//color = color_cal_rgb(mixer->ambient.color, mixer->ambient.a_light);
	if (trace_hardshadow(mixer, col_sum, origin, ray))
	{
		col_sum->diff = diffuse_main(mixer, NULL, &cp);
		//col_sum->diff = rgbof_cast_vector(mixer->ambient.color);
		//vector_create(&col_sum->diff, 127, 127, 127);
		col_sum->sw = true;	
	}
	//if (col_sum->sw)
		color = sumup_light(mixer, col_sum);
	bounces = 0;
	return (color);
}
