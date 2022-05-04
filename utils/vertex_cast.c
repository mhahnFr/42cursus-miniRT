/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_cast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:52:05 by mhahn             #+#    #+#             */
/*   Updated: 2022/03/09 19:52:09 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "vertex.h"
#include "point.h"
#include "matrix.h"
#include "vector.h"

t_matrix	*vertex3d_cast_matrix(t_vertex3d *this)
{
	t_matrix	*ret;

	ret = matrix_new(NULL, 4, 1);
	if (ret == NULL)
		return (NULL);
	ret->values[0][0] = this->x;
	ret->values[0][1] = this->y;
	ret->values[0][2] = this->z;
	ret->values[0][3] = this->w;
	return (ret);
}

t_point	*vertex3d_cast_new_point(t_vertex3d *this)
{
	t_point	*ret;

	if (this == NULL)
		return (NULL);
	ret = point_new((long) this->x, (long) this->y);
	return (ret);
}

void	vertex3d_cast_point(t_vertex3d *this, t_point *point)
{
	if (this == NULL || point == NULL)
		return ;
	point_create(point, (long) this->x, (long) this->y);
	point->r = this->r;
	point->g = this->g;
	point->b = this->b;
}

t_vector	*vertex3d_cast_new_vector(t_vertex3d *this)
{
	t_vector	*ret;

	if (this == NULL)
		return (NULL);
	ret = vector_new(this->x, this->y, this->z);
	return (ret);
}

void	vertex3d_cast_vector(t_vertex3d *this, t_vector *vector)
{
	if (this == NULL || vector == NULL)
		return ;
	vector_create(vector, this->x, this->y, this->z);
}
