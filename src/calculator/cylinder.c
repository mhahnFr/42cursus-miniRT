/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 13:54:54 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/11 14:46:25 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "libft.h"
#include <math.h>
#include <stdbool.h>
#include <float.h>
#include "vector.h"

float	cylinder_part_a(t_vector *d, t_vector *d_c)
{
	t_vector	*inter;

	vector_cross_product(inter, d, d_c);
	return (vector_scalar_product(inter, inter));
}

float	cylinder_part_b(t_vector *d, t_vector *d_c, t_vector *p, t_vector *pc)
{
	t_vector	*inter;
	t_vector	*inter2;

	vector_substract(inter2, p, p_c);
	vector_cross_product(inter, inter2, d_c);
	vector_cross_product(inter2, d, d_c);
	return (vector_scalar_product(inter, inter2));
}

float	cylinder_part_c(t_vector p, t_vector p_c, t_vector d_c, float radius)
{
	t_vector	*inter;
	t_vector	*inter2;

	vector_substract(inter, p, p_c);
	vector_cross_product(inter2, inter, d_c);
	return (vector_scalar_product(inter2, inter2) - powf(radius, 2));
}

bool	hit_cylinder(
			t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *inter)
{
	float	a;
	float	b;
	float	c;
	float	d;
	float	root;
	
	a = cylinder_part_a(ray, obj->normal);
	b = (ray, obj->normal, origin, obj->position);
	c = (origin, obj->position, obj->normal, obj->width);
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
		//needs inter filled with ray * d
		//needs col_normal vector
		//needs eventual normal flip
		return (true);
	}
	d = ((-b) + root) / a;
	if (d  < FLT_MAX && d > 0.001)
	{
		obj->disthit = d;
		return (true);
	}
	return (false);
}
//-b +- sqrtf(bˆ2 - 4ac) / 2a;
