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
#include <stdio.h>
#include <stdbool.h>
#include <float.h>
#include "vector.h"

inline float	cylinder_part_a(t_vector *d, t_vector *d_c)
{
	t_vector	inter;

	vector_cross_product(&inter, d, d_c);
	return (vector_scalar_product(&inter, &inter));
}

inline float	cylinder_part_b(t_vector *d, t_vector *d_c, t_vector *p, t_vector *pc)
{
	t_vector	inter;
	t_vector	inter2;

	vector_substract(&inter2, p, pc);
	vector_cross_product(&inter, &inter2, d_c);
	vector_cross_product(&inter2, d, d_c);
	return (2 * vector_scalar_product(&inter, &inter2));
}

inline float	cylinder_part_c(t_vector p, t_vector p_c, t_vector d_c, float radius)
{
	t_vector	inter;
	t_vector	inter2;

	vector_substract(&inter, &p, &p_c);
	vector_cross_product(&inter2, &inter, &d_c);
	return (vector_scalar_product(&inter2, &inter2) - powf(radius, 2));
}

/*bool	cylinder_length_check(t_obj_l *self, t_vector *sect)
{
	t_vector	vec;
	t_vector	inter1;

	vector_multiply_digit(&vec, &self->normal, self->height);
	vector_substract(&inter1, &self->position, sect);
	float angle = vector_scalar_product(&inter1, &vec) / (vector_length(&inter1) * vector_length(&vec));
	angle = acosf(angle);
	printf("%f\n", angle);
	return (angle > self->max_angle && angle < 90);
}*/

bool	cylinder_length_check(t_obj_l *self, t_vector *sect)
{
	t_vector	inter;
	t_vector	top_p;
	float		product;
	float		product2;

	vector_substract(&inter, sect, &self->position);
	return (vector_scalar_product(&inter, &self->normal) > 0 && vector_scalar_product(&inter, &self->normal) <= self->height);

	vector_substract(&inter, sect, &self->position);
	vector_multiply_digit(&top_p, &self->normal, self->height);
	vector_addition(&top_p, &top_p, &self->position);
	product = vector_scalar_product(&inter, &top_p);
	vector_substract(&top_p, &self->position, &top_p);
	vector_substract(&inter, sect, &top_p);
	product2 = vector_scalar_product(&inter, &top_p);
	return (product <= self->height && product2 <= self->height);
}

t_vector	cylinder_intersect_normal(t_vector *origin, t_vector *inter, t_vector *normal, float width)
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
	float 		d;

	obj_normal = obj->normal;
	vector_multiply_digit(&obj_normal, &obj->normal, -1);
	vector_multiply_digit(&obj_position, &obj_normal, obj->height);
	vector_addition(&obj_position, &obj_position, &obj->position);
	vector_substract(&inter, &obj_position, origin);
	d = vector_scalar_product(&inter, &obj_normal) / vector_scalar_product(ray, &obj_normal);
	obj->disthit = d;
	//if (vector_scalar_product(ray, &obj_normal) > 0)
	//vector_multiply_digit(&obj->col_normal, &obj_normal, -1);
	//else
	obj->col_normal = obj_normal;
	vector_multiply_digit(&inter, ray, d);
	vector_addition(sect, origin, &inter);
	vector_substract(&inter, sect, &obj_position);
	return (vector_length(&inter) < obj->width);
}

	//obj->position;
bool	hit_cylinder_bottom(
		t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *sect)
{
	t_vector	obj_normal;
	t_vector	inter;
	t_vector	obj_position;
	float 		d;

	obj_normal = obj->normal;
	obj_position = obj->position;
	//vector_multiply_digit(&obj_normal, &obj->normal, -1);
//	vector_multiply_digit(&obj_position, &obj_normal, obj->height);
//	vector_addition(&obj_position, &obj_position, &obj->position);
	vector_substract(&inter, &obj_position, origin);
	d = vector_scalar_product(&inter, &obj_normal) / vector_scalar_product(ray, &obj_normal);
	obj->disthit = d;
	//if (vector_scalar_product(ray, &obj_normal) > 0)
	//vector_multiply_digit(&obj->col_normal, &obj_normal, -1);
	//else
	obj->col_normal = obj_normal;
	vector_multiply_digit(&inter, ray, d);
	vector_addition(sect, origin, &inter);
	vector_substract(&inter, sect, &obj_position);
	return (vector_length(&inter) < obj->width);

/*
	obj_normal = obj->normal;
	//vector_multiply_digit(&obj_normal, &obj->normal, -1);
	vector_substract(&inter, &obj->position, origin);
	d = vector_scalar_product(&inter, &obj_normal) / vector_scalar_product(ray, &obj_normal);
	obj->disthit = d;
	//if (vector_scalar_product(ray, &obj_normal) > 0)
	vector_multiply_digit(&obj->col_normal, &obj_normal, -1);
	//else
	//	obj->col_normal = obj_normal;
		//return d >= 0.01f;
	vector_multiply_digit(&inter, ray, d);
	vector_addition(sect, origin, &inter);
	vector_substract(&inter, sect, &obj->position);
	return (vector_length(&inter) < obj->width);*/
}

bool	hit_cylinder_caps(
			t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *sect)
{
	t_vector	inter;
	bool		top;
	bool		bot;

	top = hit_cylinder_top(origin, obj, ray, sect);
	if (top)
		inter = *sect;
	bot = hit_cylinder_bottom(origin, obj, ray, sect);
	if (top && bot && vector_length(&inter) < vector_length(sect))
		*sect = inter;
	return (top || bot);
}

bool	hit_cylinder_mantel(
			t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *inter)
{
	float	a;
	float	b;
	float	c;
	float	d;
	float	root;

	a = cylinder_part_a(ray, &obj->normal);
	b = cylinder_part_b(ray, &obj->normal, origin, &obj->position);
	c = cylinder_part_c(*origin, obj->position, obj->normal, obj->width);
	root = powf(b, 2);
	root -= a * c * 4;
	if (root < 0)
		return (false);
	root = sqrtf(root);
	a *= 2;
	d = ((-b) - root) / a;
	if (d  < FLT_MAX && d > 0.001)
	{
		obj->disthit = d;
		vector_multiply_digit(inter, ray, d);
		if (!cylinder_length_check(obj, inter))
			return (false);
		obj->col_normal = cylinder_intersect_normal(&obj->position, inter, &obj->normal, obj->width);
		//needs eventual normal flip
		return (true);
	}
	d = ((-b) + root) / a;
	if (d  < FLT_MAX && d > 0.001)
	{
		obj->disthit = d;
		vector_multiply_digit(inter, ray, d);
		if (!cylinder_length_check(obj, inter))
			return (false);
		obj->col_normal = cylinder_intersect_normal(&obj->position, inter, &obj->normal, obj->width);
		return (true);
	}
	return (false);
}

//-b +- sqrtf(bˆ2 - 4ac) / 2a;

bool	hit_cylinder(
			t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *sect)
{
	t_vector	inter;
	bool		mat;
	bool		cap;

	mat = hit_cylinder_mantel(origin, obj, ray, sect);
	if (mat)
		inter = *sect;
	cap = hit_cylinder_caps(origin, obj, ray, sect);
	if (mat && cap && vector_length(&inter) < vector_length(sect))
		*sect = inter;
	return (mat || cap);
}
