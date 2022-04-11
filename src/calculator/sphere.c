/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:40:43 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/11 14:18:43 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>
#include <float.h>

bool	hit_sphere(t_vector	*origin, t_obj_l *obj, t_vector *ray, t_vector *ret)
{
	float		a;
	float		b;
	float		c;
	float		d;
	float		temp;
	t_vector	oc;

	vector_substract(&oc, origin, &obj->position);
	a = vector_scalar_product(ray, ray);
	b = vector_scalar_product(&oc, ray);
	c = vector_scalar_product(&oc, &oc) - powf(obj->height, 2);
	d = powf(b, 2) - a * c;
	if (d < 0)
		return (false);
	temp = (-b - sqrtf(d)) / a;
	if (temp < FLT_MAX && temp > 0.001f)
	{
		obj->disthit = temp;
		vector_multiply_digit(&oc, ray, temp);
		vector_addition(ret, origin, &oc);
		vector_substract(&oc, ret, &obj->position);
		vector_division(&obj->col_normal, &oc, obj->height);
		if (obj->inv_normal)
			vector_multiply_digit(&obj->col_normal, &obj->col_normal, -1);
		return (true);
	}
	temp = (-b + sqrtf(d)) / a;
	if (temp < FLT_MAX && temp > 0.001f)
	{
		obj->disthit = temp;
		vector_multiply_digit(&oc, ray, temp);
		vector_addition(ret, origin, &oc);
		vector_substract(&oc, ret, &obj->position);
		vector_division(&obj->col_normal, &oc, obj->height);
		if (obj->inv_normal)
			vector_multiply_digit(&obj->col_normal, &obj->col_normal, -1);
		return (true);
	}
	return (false);
}
