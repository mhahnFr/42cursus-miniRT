/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:40:11 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/08 15:32:39 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	intersec_plane(
		t_vector *cam, t_vector *cam_pos, t_obj_l *obj, t_vector *point)
{
	float		t;
	t_vector	inter;

	vector_substract(&inter, &obj->position, cam_pos);
	t = vector_scalar_product(&inter, &obj->normal)
		/ vector_scalar_product(cam, &obj->normal);
	obj->disthit = t;
	if (vector_scalar_product(cam, &obj->normal) > 0)
		vector_multiply_digit(&obj->col_normal, &obj->normal, -1);
	else
		obj->col_normal = obj->normal;
	vector_multiply_digit(&inter, cam, t);
	vector_addition(point, cam_pos, &inter);
	return (t >= 0.01f);
}

bool	fast_intersec_plane(t_vector *vec, t_vector *normal)
{
	return (vector_scalar_product(vec, normal) != 0);
}
