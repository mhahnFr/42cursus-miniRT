/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 13:54:54 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/20 16:05:32 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>
#include <stdbool.h>
#include <float.h>
#include <stdio.h>
#include "vector.h"

inline float	cylinder_part_a(t_vector *d, t_vector *d_c)
{
	t_vector	inter;

	vector_cross_product(&inter, d, d_c);
	return (vector_scalar_product(&inter, &inter));
}

inline float	cylinder_part_b(t_vector *d,
								t_vector *d_c,
								t_vector *p,
								t_vector *pc)
{
	t_vector	inter;
	t_vector	inter2;

	vector_substract(&inter2, p, pc);
	vector_cross_product(&inter, &inter2, d_c);
	vector_cross_product(&inter2, d, d_c);
	return (2 * vector_scalar_product(&inter, &inter2));
}

inline float	cylinder_part_c(t_vector p,
								t_vector p_c,
								t_vector d_c,
								float radius)
{
	t_vector	inter;
	t_vector	inter2;

	vector_substract(&inter, &p, &p_c);
	vector_cross_product(&inter2, &inter, &d_c);
	return (vector_scalar_product(&inter2, &inter2) - powf(radius, 2));
}

bool	cylinder_length_check(t_obj_l *self, t_vector *sect)
{
	t_vector	inter;

	vector_substract(&inter, sect, &self->position);
	return (vector_scalar_product(&inter, &self->normal) \
	> 0 && vector_scalar_product(&inter, &self->normal) <= self->height);
}

inline t_vector	cylinder_intersect_normal(t_vector *origin,
											t_vector *inter,
											t_vector *normal,
											bool inv)
{
	float		t;
	t_vector	res;

	vector_substract(&res, inter, origin);
	t = vector_scalar_product(&res, normal);
	vector_multiply_digit(&res, normal, t);
	vector_addition(&res, &res, origin);
	vector_substract(&res, inter, &res);
	vector_normalize(&res);
	if (inv)
		vector_multiply_digit(&res, &res, -1);
	return (res);
}

bool	hit_cylinder_part_b_a(t_test cy_struct)
{
	cy_struct.d = ((-cy_struct.b) - cy_struct.root) / cy_struct.a;
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

//-b +- sqrtf(bˆ2 - 4ac) / 2a;
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

//obj->position;
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

	top = hit_cylinder_top(origin, obj, ray, sect);
	if (top)
	{
		inter = *sect;
		save_norm = obj->col_normal;
	}
	bot = hit_cylinder_bottom(origin, obj, ray, sect);
	if (!bot && (top && vector_length(&inter) < vector_length(sect)))
	{
		*sect = inter;
		obj->col_normal = save_norm;
	}
	return (top || bot);
}

bool	hit_cylinder(
		t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *sect)
{
	t_vector	inter;
	t_vector	save_norm;
	bool		mat;
	bool		cap;

	mat = hit_cylinder_mantel(origin, obj, ray, sect);
	if (mat)
	{
		inter = *sect;
		save_norm = obj->col_normal;
	}
	cap = hit_cylinder_caps(origin, obj, ray, sect);
	if (!cap || (mat && vector_length(&inter) < vector_length(sect)))
	{
		*sect = inter;
		obj->col_normal = save_norm;
	}
	return (mat || cap);
}
