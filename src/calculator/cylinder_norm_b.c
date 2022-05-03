/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_norm_b.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 11:30:15 by mhahn             #+#    #+#             */
/*   Updated: 2022/05/03 11:30:18 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <float.h>
#include "minirt.h"

bool	hit_cylinder_part_b_a(t_test cy_struct)
{
	cy_struct.d = ((-cy_struct.b) - cy_struct.root) / cy_struct.a;
	if (cy_struct.d < FLT_MAX && cy_struct.d > 0.01f)
	{
		cy_struct.obj->disthit = cy_struct.d;
		vector_multiply_digit(cy_struct.inter, cy_struct.ray, cy_struct.d);
		vector_addition(cy_struct.inter, cy_struct.inter, cy_struct.origin);
		if (!cylinder_length_check(cy_struct.obj, cy_struct.inter))
			return (false);
		cy_struct.obj->col_normal = cylinder_intersect_normal(\
		&cy_struct.obj->position, cy_struct.inter, \
		&cy_struct.obj->normal, cy_struct.obj->inv_normal);
		return (true);
	}
	return (false);
}

bool	cylinder_length_check(t_obj_l *self, t_vector *sect)
{
	t_vector	inter;

	vector_substract(&inter, sect, &self->position);
	return (vector_scalar_product(&inter, &self->normal) \
	> 0 && vector_scalar_product(&inter, &self->normal) <= self->height);
}
