/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baseimage.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:23:59 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/29 20:55:42 by jkasper          ###   ########.fr       */
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
	vector_multiply(&b, inter, &a);
	vector_multiply_digit(inter, &b, light->brightness);
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
	fac_sum = 0;
	vector_create(&final_color, 0, 0, 0);
	while (i < col_sum->l_count)
	{
		fac_sum += col_sum->fac[i];
		i++;
	}
	fac_sum += mixer->ambient.a_light;
	fac_sum = 1 / fac_sum;
	i = 0;
	while (i < col_sum->l_count)
	{
		vector_multiply_digit(&(col_sum->sum[i]), &(col_sum->sum[i]), (col_sum->fac[i] * fac_sum));
		vector_addition(&final_color, &final_color, &(col_sum->sum[i]));
		i++;
	}
	if (!col_sum->sw)
	{
		inter = rgbof_cast_vector(mixer->ambient.color);
		vector_multiply_digit(&inter, &inter, mixer->ambient.a_light * fac_sum);
	}
	else
		vector_multiply_digit(&inter, &col_sum->diff, mixer->ambient.a_light * fac_sum);
	vector_addition(&final_color, &final_color, &inter);
	color = vector_cast_rgbof(final_color);
	return (color);
}

bool	trace_light(t_mixer *mixer, t_obj_l *curr, t_col *col_sum, t_vector intersect)
{
	t_vector	inter;
	t_vector	ray;
	t_vector	added;
	
	bool		ret;
	
	t_obj_l		*l;
	ret = true;
	col_sum->l_count = 0;
	l = mixer->obj_list;
	while (l != NULL)
	{
		if (l->obj_type == LIGHT)
		{
			vector_substract(&ray, &l->position, &intersect);
			vector_normalize(&ray);
			if (!intersect_object(mixer, curr, &intersect, l, ray, &added))
			{
				col_sum->sum[col_sum->l_count] = added;
				col_sum->fac[col_sum->l_count++] = l->brightness;
				ret = false;
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
		return trace_light(mixer, curr, colsum, intersect2);
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
	color = color_cal_rgb(mixer->ambient.color, mixer->ambient.a_light);
	if (trace_hardshadow(mixer, col_sum, origin, ray) || col_sum->l_count != mixer->light_count)
	{
		col_sum->diff = diffuse_main(mixer, NULL, &cp);
		col_sum->sw = true;	
	}
	if (col_sum->l_count > 0 || col_sum->sw)
		color = sumup_light(mixer, col_sum);
	return (color);
}
