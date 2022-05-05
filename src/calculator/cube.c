/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 18:26:15 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/20 15:49:07 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline bool	hit_cube_area(t_vector normal, t_vector inter, float length)
{
	return (vector_scalar_product(&inter, &normal) >= \
	(length / 2) || vector_scalar_product(&inter, &normal) <= -(length / 2));
}

//0 top 1 bottom 2 left 3 right 4 behind 5 front
static inline bool	determine_area(t_obj_l *obj, t_vector inter, \
									t_vector *intersec)
{
	vector_substract(&inter, intersec, &obj->position);
	if (obj->cube_side < 4)
		if (hit_cube_area(obj->normal_down, inter, obj->depth))
			return (false);
	if (obj->cube_side > 1)
		if (hit_cube_area(obj->normal_left, inter, obj->height))
			return (false);
	if (obj->cube_side != 2 && obj->cube_side != 3)
	{
		if ((obj->cube_side < 2 && hit_cube_area(obj->normal_left, inter, \
		obj->width)) || (obj->cube_side > 3 && hit_cube_area(\
		obj->normal_down, inter, obj->width)))
			return (false);
	}
	return (true);
}

bool	hit_cube(t_vector *origin, t_obj_l *obj, t_vector *ray, \
				t_vector *intersec)
{
	float		t;
	t_vector	inter;

	if (vector_scalar_product(ray, &obj->normal) == 0)
		return (false);
	vector_substract(&inter, &obj->position, origin);
	t = vector_scalar_product(&inter, &obj->normal)
		/ vector_scalar_product(ray, &obj->normal);
	obj->disthit = t;
	if (vector_scalar_product(ray, &obj->normal) > 0)
		vector_multiply_digit(&obj->col_normal, &obj->normal, -1);
	else
		obj->col_normal = obj->normal;
	vector_multiply_digit(&inter, ray, t);
	vector_addition(intersec, origin, &inter);
	if (t < 0.01f)
		return (false);
	return (determine_area(obj, inter, intersec));
}
