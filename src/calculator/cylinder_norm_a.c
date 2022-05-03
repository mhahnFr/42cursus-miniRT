/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_norm_a.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 11:30:10 by mhahn             #+#    #+#             */
/*   Updated: 2022/05/03 11:30:13 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <float.h>
#include <math.h>
#include "minirt.h"

bool	hit_cylinder_part_b_b(t_test cy_struct)
{
	cy_struct.d = ((-cy_struct.b) + cy_struct.root) / cy_struct.a;
	if (cy_struct.d < FLT_MAX && cy_struct.d > 0.01f)
	{
		cy_struct.obj->disthit = cy_struct.d;
		vector_multiply_digit(cy_struct.inter, cy_struct.ray, cy_struct.d);
		vector_addition(cy_struct.inter, cy_struct.inter, cy_struct.origin);
		if (!cylinder_length_check(cy_struct.obj, cy_struct.inter))
			return (false);
		cy_struct.obj->col_normal = cylinder_intersect_normal(\
		&cy_struct.obj->position, cy_struct.inter, \
		&cy_struct.obj->normal, cy_struct.obj->inv_normal);
		return (true);
	}
	return (false);
}

bool	hit_cylinder_mantel(t_vector *origin,
							t_obj_l *obj,
							t_vector *ray,
							t_vector *inter)
{
	t_test	cy_struct;

	cy_struct.obj = obj;
	cy_struct.ray = ray;
	cy_struct.origin = origin;
	cy_struct.inter = inter;
	cy_struct.a = cylinder_part_a(ray, &obj->normal);
	cy_struct.b = cylinder_part_b(ray, &obj->normal, origin, &obj->position);
	cy_struct.c = cylinder_part_c(*origin, obj->position, obj->normal, \
	obj->width);
	cy_struct.root = powf(cy_struct.b, 2);
	cy_struct.root -= cy_struct.a * cy_struct.c * 4;
	if (cy_struct.root < 0)
		return (false);
	cy_struct.root = sqrtf(cy_struct.root);
	cy_struct.a *= 2;
	return (hit_cylinder_part_b_a(cy_struct) || \
	hit_cylinder_part_b_b(cy_struct));
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
	d = vector_scalar_product(&inter, &obj_normal) / \
	vector_scalar_product(ray, &obj_normal);
	obj->disthit = d;
	obj->col_normal = obj_normal;
	if (obj->inv_normal)
		vector_multiply_digit(&obj->col_normal, &obj->col_normal, -1);
	vector_multiply_digit(&inter, ray, d);
	vector_addition(sect, origin, &inter);
	vector_substract(&inter, sect, &obj_position);
	return (vector_length(&inter) < obj->width && d < FLT_MAX && d >= 0.01f);
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
	d = vector_scalar_product(&inter, &obj_normal) / \
	vector_scalar_product(ray, &obj_normal);
	obj->disthit = d;
	if (obj->inv_normal)
		vector_multiply_digit(&obj->col_normal, &obj_normal, 1);
	else
		vector_multiply_digit(&obj->col_normal, &obj_normal, -1);
	vector_multiply_digit(&inter, ray, d);
	vector_addition(sect, origin, &inter);
	vector_substract(&inter, sect, &obj_position);
	return (vector_length(&inter) < obj->width && d < FLT_MAX && d >= 0.01f);
}

bool	hit_cylinder_caps(
		t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *sect)
{
	t_vector	inter;
	t_vector	save_norm;
	bool		top;
	bool		bot;
	float		disthit;

	top = hit_cylinder_top(origin, obj, ray, sect);
	if (top)
	{
		inter = *sect;
		save_norm = obj->col_normal;
		disthit = obj->disthit;
	}
	bot = hit_cylinder_bottom(origin, obj, ray, sect);
	if (!bot && (top && vector_length(&inter) < vector_length(sect)))
	{
		*sect = inter;
		obj->col_normal = save_norm;
		obj->disthit = disthit;
	}
	return (top || bot);
}
