/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:40:43 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/20 16:04:46 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>
#include <float.h>

static inline bool	hit_sphere2(struct s_anti_norm *s)
{
	s->obj->disthit = s->temp;
	vector_multiply_digit(&s->oc, s->ray, s->temp);
	vector_addition(s->ret, s->origin, &s->oc);
	vector_substract(&s->oc, s->ret, &s->obj->position);
	vector_division(&s->obj->col_normal, &s->oc, s->obj->height);
	if (s->obj->inv_normal)
		vector_multiply_digit(&s->obj->col_normal, &s->obj->col_normal, -1);
	return (true);
}

static inline bool	hit_sphere1(struct s_anti_norm *s, bool *second)
{
	*second = false;
	vector_substract(&s->oc, s->origin, &s->obj->position);
	s->a = vector_scalar_product(s->ray, s->ray);
	s->b = vector_scalar_product(&s->oc, s->ray);
	s->c = vector_scalar_product(&s->oc, &s->oc) - powf(s->obj->height, 2);
	s->d = powf(s->b, 2) - s->a * s->c;
	if (s->d < 0)
		return (false);
	s->temp = (-s->b - sqrtf(s->d)) / s->a;
	*second = true;
	return (s->temp < FLT_MAX && s->temp > 0.001f);
}

bool	hit_sphere(t_vector	*origin, t_obj_l *obj, t_vector *ray, t_vector *ret)
{
	struct s_anti_norm	s;
	bool				second;

	s.origin = origin;
	s.obj = obj;
	s.ray = ray;
	s.ret = ret;
	if (hit_sphere1(&s, &second) && second)
		return (hit_sphere2(&s));
	else
	{
		s.temp = (-s.b + sqrtf(s.d)) / s.a;
		if (s.temp < FLT_MAX && s.temp > 0.001f)
			return (hit_sphere2(&s));
	}
	return (false);
}
