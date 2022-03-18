/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:40:43 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/18 22:25:20 by mhahn            ###   ########.fr       */
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
	t_vector	OC;
	
	vector_substract(&OC, origin, &(obj->position));
	a = vector_scalar_product(ray, ray);
	b = vector_scalar_product(&OC, ray);
	c = vector_scalar_product(&OC, &OC) - powf(obj->height, 2);
	d = powf(b,2) - a *c;
	if (d < 0)
		return (false);
	float	temp = (-b - sqrtf(d) ) / a;
	if (temp < FLT_MAX && temp > 0)
	{
		obj->disthit = temp;
		vector_multiply_digit(&OC, ray, temp);
		vector_addition(ret, origin, &OC);
		vector_substract(&OC, ret, &(obj->position));
		vector_division(&(obj->col_normal), &OC, obj->height);
		return (true);
	}
	temp = (-b + sqrtf(d)) / a;
	if (temp < FLT_MAX && temp > 0)
	{
		obj->disthit = temp;
		vector_multiply_digit(&OC, ray, temp);
		vector_addition(ret, origin, &OC);
		vector_substract(&OC, ret, &(obj->position));
		vector_division(&(obj->col_normal), &OC, obj->height);
		return (true);
	}
	return (false);
}
