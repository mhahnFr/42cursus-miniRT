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
	return (vector_scalar_product(&inter, &inter2));
}

inline float	cylinder_part_c(t_vector p, t_vector p_c, t_vector d_c, float radius)
{
	t_vector	inter;
	t_vector	inter2;

	vector_substract(&inter, &p, &p_c);
	vector_cross_product(&inter2, &inter, &d_c);
	return (vector_scalar_product(&inter2, &inter2) - powf(radius, 2));
}

t_vector	cylinder_intersect_normal(t_vector *origin, t_vector *inter, t_vector *normal, float width)
{
	float		a;
	float		b;
	float		c;
	t_vector	ret;

	a = powf(width, 2);
	vector_substract(&ret, inter, origin);
	c = powf(vector_length(&ret), 2);
	b = c - a;
	b = sqrtf(b);
	vector_multiply_digit(&ret, normal, b);
	vector_substract(&ret, &ret, inter);
	vector_normalize(&ret);
	return (ret);
}

bool	hit_cylinder(
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
	root -= 4 * a * c;
	if (root <= 0)
		return (false);
	root = sqrtf(root);
	a *= 2;
	d = ((-b) - root) / a;
	if (d  < FLT_MAX && d > 0.001)
	{
		obj->disthit = d;
		vector_multiply_digit(inter, ray, d);
		obj->col_normal = cylinder_intersect_normal(origin, inter, &obj->normal, obj->width);
		//needs eventual normal flip
		return (true);
	}
	d = ((-b) + root) / a;
	if (d  < FLT_MAX && d > 0.001)
	{
		obj->disthit = d;
		vector_multiply_digit(inter, ray, d);
		obj->col_normal = cylinder_intersect_normal(origin, inter, &obj->normal, obj->width);
		return (true);
	}
	return (false);
}
//-b +- sqrtf(bˆ2 - 4ac) / 2a;
