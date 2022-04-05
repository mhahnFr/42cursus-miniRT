/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baseimage.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:23:59 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/05 19:06:22 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "vector.h"
#include <math.h>

bool	intersec_next(t_thread *self, t_obj_l *objs, t_vector *origin, t_vector *ray, t_vector *inter)
{
	bool	ret;

	ret = false;
	if (objs->obj_type == SPHERE)
		ret = hit_sphere(self, origin, objs, ray, inter);
	else if (objs->obj_type == PLANE && fast_intersec_plane(ray, &objs->normal))
	{
		ret = intersec_plane(self, ray, origin, objs, inter);
		self->disthit = vector_distance(origin, inter);
	}
	else if (objs->obj_type == LIGHT)
		ret = specular_highlight(self, origin, objs, ray, inter);
	//else if (objs->obj_type == CYLINDER)
	//	return (calc_intersection_cylinder(origin, objs, ray, inter))
	return (ret);
}

bool	intersect_object(t_thread *self, t_obj_l *nointersec, t_vector *origin, t_obj_l *light, t_vector ray, t_vector *color)
{
	t_vector	intersect;
	t_vector	inter;
	t_obj_l		*curr;
	t_obj_l		*list;
	float		distsf;
	bool		sw;

	sw = false;
	curr = NULL;
	list = self->mixer->obj_list;
	while (list != NULL)
	{
		if (sw == false && list->obj_type != LIGHT && intersec_next(self, list, origin, &ray, &intersect))
		{
			inter = intersect;
			distsf = self->disthit;
			sw = true;
			curr = list;
		}
		else if (sw == true && list->obj_type != LIGHT && intersec_next(self, list, origin, &ray, &intersect) && distsf > self->disthit)
		{
			inter = intersect;
			distsf = self->disthit;
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
	int	i;

	vector_create(&final_color, 0, 0, 0);;
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

t_vector	trace_light(t_thread *self, t_obj_l *curr, t_col *col_sum, t_vector intersect)
{
	t_vector	ray;
	t_vector	added;
	t_vector	sum;
	float		length;
	t_obj_l		*l;
	bool		ret;
	
	ret = true;
	vector_create(&sum, 0,0,0);
	col_sum->l_count = 0;
	l = self->mixer->obj_list;
	col_sum->basecolor = rgbof_cast_vector(curr->color);
	while (l != NULL)
	{
		if (l->obj_type == LIGHT)
		{
			vector_substract(&ray, &l->position, &intersect);
			length = vector_length(&ray);
			vector_normalize(&ray);
			intersect_object(self, curr, &intersect, l, ray, &added);
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

t_vector	trace_next(t_thread *self, t_vector intersect, t_vector ray, t_obj_l *curr)
{
	ray = trace_rand(ray, self->obj_int_normal, curr->diffusion);
	return (rgbof_cast_vector(calc_shader(&intersect, &ray, self, &self->col_sum)));
}

bool	trace_hardshadow(t_thread *self, t_col *colsum, t_vector *origin, t_vector *ray)
{
	t_vector	intersect;
	t_vector	intersect2;
	t_obj_l		*curr;
	float		distsf;
	bool		sw;
	t_obj_l		*list;

	sw = false;
	curr = NULL;
	list = self->mixer->obj_list;
	while (list != NULL)
	{
		if (sw == false && intersec_next(self, list, origin, ray, &intersect))
		{
			distsf = self->disthit;
			sw = true;
			curr = list;
			intersect2 = intersect;
		}
		else if (sw == true && intersec_next(self, list, origin, ray, &intersect) && distsf > self->disthit)
		{
			distsf = self->disthit;
			curr = list;
			intersect2 = intersect;
		}
		list = list->next;
	}
	self->disthit = distsf;
	if (curr != NULL && !(self->bounces < 2 && curr->obj_type == LIGHT))
	{
		if (curr->obj_type == LIGHT)
		{
			colsum->diff = rgbof_cast_vector(curr->color);
			colsum->sum[0] = colsum->diff;
			colsum->fac[0] = 1;
			colsum->l_count = 1;
			return (false);
		}
		colsum->diff = rgbof_cast_vector(self->mixer->ambient.color);
		intersect = rgbof_cast_vector(curr->color);
		t_vector s_col, l_col;
		vector_create(&s_col, 1, 1, 1);
		vector_create(&l_col, 1, 1, 1);
		if (curr->reflec_fac > 0)
			s_col = trace_next(self, intersect2, *ray, curr);
		if (curr->reflec_fac < 1)
			l_col = trace_light(self, curr, colsum, intersect2);
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

t_rgbof	calc_shader(t_vector *origin, t_vector *ray, t_thread *self, t_col *col_sum)
{
	t_rgbof		color;
	t_vector	cp;
	t_mixer		*mixer;

	cp.x = ray->x;
	cp.y = ray->y;
	cp.z = ray->z;
	mixer = self->mixer;
	self->bounces++;
	if (self->bounces == MAX_BOUNCES + 1)
		return (mixer->ambient.color);
	color = color_rgb(mixer->ambient.color.r , mixer->ambient.color.g, mixer->ambient.color.g);
	col_sum->diff = rgbof_cast_vector(color);
	if (trace_hardshadow(self, col_sum, origin, ray))
		col_sum->diff = diffuse_main(self, NULL, &cp);
	color = sumup_light(self->mixer, col_sum);
	return (color);
}
