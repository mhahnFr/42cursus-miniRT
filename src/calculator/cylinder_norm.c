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

bool	hit_cylinder(
		t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *sect)
{
	if (obj->obj_type == CYLINDER_MAN)
		return (hit_cylinder_mantel(origin, obj, ray, sect));
	else if (obj->obj_type == CYLINDER_CAP)
		return (hit_cylinder_cap(origin, obj, ray, sect));
	return (false);
}
