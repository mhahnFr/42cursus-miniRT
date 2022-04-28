/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 18:33:07 by mhahn             #+#    #+#             */
/*   Updated: 2022/04/26 18:33:11 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>
#include <stdbool.h>
#include <float.h>
#include "vector.h"

bool	cylinder_length_check(t_obj_l *self, t_vector *sect)
{
	t_vector	inter;

	vector_substract(&inter, sect, &self->position);
	return (vector_scalar_product(&inter, &self->normal) > 0
		&& vector_scalar_product(&inter, &self->normal) <= self->height);
}

t_vector	cylinder_intersect_normal(
		t_vector *origin, t_vector *inter, t_vector *normal, float width)
{
	float		a;
	float		b;
	float		c;
	t_vector	ret;

	a = powf(width, 2);
	vector_substract(&ret, origin, inter);
	c = powf(vector_length(&ret), 2);
	b = c - a;
	b = sqrtf(b);
	vector_multiply_digit(&ret, normal, b);
	vector_substract(&ret, &ret, inter);
	vector_normalize(&ret);
	vector_multiply_digit(&ret, &ret, 1);
	return (ret);
}

bool	hit_cylinder_top(
		t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *sect)
{
	t_vector	obj_normal;
	t_vector	obj_position;
	t_vector	inter;
	float		d;

	obj_normal = obj->normal;
	vector_multiply_digit(&obj_position, &obj_normal, obj->height);
	vector_addition(&obj_position, &obj_position, &obj->position);
	vector_substract(&inter, &obj_position, origin);
	d = vector_scalar_product(&inter, &obj_normal)
		/ vector_scalar_product(ray, &obj_normal);
	obj->disthit = d;
	obj->col_normal = obj_normal;
	vector_multiply_digit(&obj->col_normal, &obj->col_normal, 1);
	vector_multiply_digit(&inter, ray, d);
	vector_addition(sect, origin, &inter);
	vector_substract(&inter, sect, &obj_position);
	return (vector_length(&inter) < obj->width);
}

bool	hit_cylinder_bottom(
		t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *sect)
{
	t_vector	obj_normal;
	t_vector	inter;
	t_vector	obj_position;
	float		d;

	obj_normal = obj->normal;
	obj_position = obj->position;
	vector_substract(&inter, &obj_position, origin);
	d = vector_scalar_product(&inter, &obj_normal)
		/ vector_scalar_product(ray, &obj_normal);
	obj->disthit = d;
	vector_multiply_digit(&obj->col_normal, &obj_normal, 1);
	vector_multiply_digit(&inter, ray, d);
	vector_addition(sect, origin, &inter);
	vector_substract(&inter, sect, &obj_position);
	return (vector_length(&inter) < obj->width);
}

bool	hit_cylinder_caps(
		t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *sect)
{
	t_vector	inter;
	t_vector	save_norm;
	bool		top;
	bool		bot;

	top = hit_cylinder_top(origin, obj, ray, sect);
	if (top)
	{
		inter = *sect;
		save_norm = obj->col_normal;
	}
	bot = hit_cylinder_bottom(origin, obj, ray, sect);
	if (!bot || (top && vector_length(&inter) < vector_length(sect)))
	{
		*sect = inter;
		obj->col_normal = save_norm;
	}
	return (top || bot);
}
