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

static inline bool	hit_cylinder1(struct s_test *s, bool *second)
{
	s->a = cylinder_part_a(s->ray, &s->obj->normal);
	s->b = cylinder_part_b(s->ray, &s->obj->normal,
			s->origin, &s->obj->position);
	s->c = cylinder_part_c(*(s->origin), s->obj->position,
			s->obj->normal, s->obj->width);
	s->root = powf(s->b, 2);
	s->root -= s->a * s->c * 4;
	*second = false;
	if (s->root < 0)
		return (false);
	s->root = sqrtf(s->root);
	s->a = s->a * 2;
	s->d = ((-s->b) - s->root) / s->a;
	*second = true;
	return (s->d < FLT_MAX && s->d > 0.001f);
}

static inline bool	hit_cylinder2(struct s_test *s)
{
	s->obj->disthit = s->d;
	vector_multiply_digit(s->inter, s->ray, s->d);
	vector_addition(s->inter, s->inter, s->origin);
	if (!cylinder_length_check(s->obj, s->inter))
		return (false);
	s->obj->col_normal = cylinder_intersect_normal(&s->obj->position, s->inter,
			&s->obj->normal, s->obj->width);
	return (true);
}

bool	hit_cylinder_mantel(
			t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *inter)
{
	struct s_test	s;
	bool			second;

	s.origin = origin;
	s.ray = ray;
	s.inter = inter;
	s.obj = obj;


	if (hit_cylinder1(&s, &second) && second)
		return (hit_cylinder2(&s));
	else if (second)
	{
		s.d = ((-s.b) + s.root) / s.a;
		if (s.d < FLT_MAX && s.d > 0.001f)
			return (hit_cylinder2(&s));
	}
	return (false);
}

#include <stdio.h>
bool	hit_cylinder(
			t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *sect)
{
	t_vector	inter;
	t_vector 	save_norm;
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
	if (mat || cap) {
		//printf("%f %f %f\n", obj->col_normal.x, obj->col_normal.y, obj->col_normal.z);
		vector_normalize(&obj->col_normal);
	}
	return (mat || cap);
}
