/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:38:20 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/08 17:33:55 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>

#include "vector.h"

t_vector	*vector_new(float x, float y, float z)
{
	t_vector	*ret;

	ret = ft_gc_malloc(sizeof(struct s_vector));
	if (ret != NULL)
		vector_create(ret, x, y, z);
	return (ret);
}

void	vector_create(t_vector *this, float x, float y, float z)
{
	if (this == NULL)
		return ;
	this->x = x;
	this->y = y;
	this->z = z;
}

float	vector_length(t_vector *this)
{
	if (this == NULL)
		return (0);
	return (sqrt(this->x * this->x + this->y * this->y + this->z * this->z));
}

void	vector_normalize(t_vector *this)
{
	float	l;

	l = 1 / vector_length(this);
	vector_multiply_digit(this, this, l);
}
