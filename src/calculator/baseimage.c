/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baseimage.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:23:59 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/31 16:17:14 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "vector.h"
#include <math.h>

t_vector	rgbof_light_strenght(t_vector *inter, t_vector *origin, t_obj_l *light, t_obj_l *ori)
{
	t_vector	a;
	t_vector	b;

	// TODO Calculate lightloss along distance
	*inter = rgbof_cast_vector(light->color);
	a = rgbof_cast_vector(ori->color);
	vector_multiply(inter, inter, &a);
	//vector_multiply_digit(inter, &b, light->brightness);
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
	if (curr == NULL)
	{
		//intersect = col;
		inter = rgbof_light_strenght(&inter, origin, light, nointersec);
		//t_vector v;
		//vector_create(&v, mixer->ambient.a_light, light->brightness, 0);
		//vector_normalize(&v);
		//vector_multiply_digit(&inter, &inter, v.y);
		//vector_multiply_digit(&col, &col, v.x);
		//vector_addition(&inter, &inter, &col);
		//vector_multiply_digit(&inter, &inter, 0.5f);
		*color = inter;
		return (false);
	}
	return (true);
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
	fac_sum = 1.0f / (mixer->light_count + 1);
	i = 0;
	while (i < col_sum->l_count)
	{
		vector_multiply_digit(&(col_sum->sum[i]), &(col_sum->sum[i]), (col_sum->fac[i] * fac_sum));
		vector_addition(&final_color, &final_color, &(col_sum->sum[i]));
		i++;
	}
	//if (i == 0)
	//final_color = col_sum->diff;
	vector_multiply_digit(&inter, &col_sum->diff, mixer->ambient.a_light * fac_sum);
	vector_addition(&final_color, &final_color, &inter);
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

bool	trace_light(t_mixer *mixer, t_obj_l *curr, t_col *col_sum, t_vector intersect)
{
	t_vector	inter;
	t_vector	ray;
	t_vector	added;
	float		length;
	
	bool		ret;
	
	t_obj_l		*l;
	ret = true;
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
				col_sum->sum[col_sum->l_count] = added;
				length = light_distance_factor(length, l->brightness, l->intensity);
				//col_sum->sum[col_sum->l_count] = rgbof_cast_vector(l->color);
				//length = l->brightness;
				if (length != 0)
				{
					col_sum->fac[col_sum->l_count++] = length;
					if (col_sum->l_count == 1)
						ret = false;
					//if (length < l->brightness)
					//	ret = true;
				}
			}
		}
		l = l->next;
	}
	return (ret);
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
	if (curr != NULL)
	{
		//printf("%f %f\n", curr->diffusion, curr->reflec_fac);
		colsum->diff = rgbof_cast_vector(mixer->ambient.color);
		intersect = rgbof_cast_vector(curr->color);
		vector_multiply(&colsum->diff, &colsum->diff, &intersect);
		return trace_light(mixer, curr, colsum, intersect2);
	}
	return (curr != NULL);
}

t_rgbof	calc_shader(t_vector *origin, t_vector *ray, t_mixer *mixer, t_col *col_sum)
{
	t_rgbof		color;
	t_vector	cp;

	cp.x = ray->x;
	cp.y = ray->y;
	cp.z = ray->z;
	col_sum->sw = false;
	color = color_rgb(mixer->ambient.color.r * mixer->ambient.a_light, mixer->ambient.color.g * mixer->ambient.a_light, mixer->ambient.color.g * mixer->ambient.a_light);
	//color = color_cal_rgb(mixer->ambient.color, mixer->ambient.a_light);
	if (trace_hardshadow(mixer, col_sum, origin, ray) || col_sum->l_count != mixer->light_count)
	{
		col_sum->diff = diffuse_main(mixer, NULL, &cp);
		col_sum->sw = true;	
	}
	if (col_sum->l_count > 0 || col_sum->sw)
		color = sumup_light(mixer, col_sum);
	return (color);
}
