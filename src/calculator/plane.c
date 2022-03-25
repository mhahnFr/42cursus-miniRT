/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:40:11 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/25 18:20:09 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


bool	intersec_plane(t_vector *cam, t_obj_l *objs, t_vector *point)
{
	t_vector	vec2;
	t_vector	vec_inter;
	float		d;

	vec2.x = cam->x;
	vec2.y = cam->y;
	vec2.z = cam->z;
	vector_normalize(&vec2);
	vector_multiply_digit(&vec_inter, &vec2, 0.5);
	vector_multiply(&vec2, &vec_inter, cam);
	vector_substract(&vec_inter, &(objs->position), &vec2);
	d = vector_scalar_product(&vec_inter, &(objs->normal)) / vector_scalar_product(cam, &(objs->normal));
	if (d < 0)
		return (false);
	vector_multiply_digit(&vec_inter, cam, d);
	vector_addition(point, &vec2, &vec_inter);
	objs->disthit = d;
	if (vector_scalar_product(cam, &objs->normal) > 0)
		vector_multiply_digit(&objs->col_normal, &objs->normal, -1);
	else
		objs->col_normal = objs->normal;
	return (true);
}

bool	fast_intersec_plane(t_vector *vec, t_vector *normal)
{
	return (vector_scalar_product(vec, normal) != 0);
}
