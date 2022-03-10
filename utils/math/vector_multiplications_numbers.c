/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_multiplications_numbers.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 00:01:30 by mhahn             #+#    #+#             */
/*   Updated: 2022/03/11 00:01:58 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "vector.h"

float	vector_scalar_product(t_vector *one, t_vector *two)
{
	if (one == NULL || two == NULL)
		return (0);
	return (one->x * two->x + one->y * two->y + one->z * two->z);
}

t_vector	*vector_new_multiply_digit(t_vector *this, float multiplier)
{
	t_vector	*ret;

	if (this == NULL)
		return (NULL);
	ret = vector_new(0, 0, 0);
	if (ret == NULL)
		return (NULL);
	vector_multiply_digit(ret, this, multiplier);
	return (ret);
}

void	vector_multiply_digit(
			t_vector *result,
			t_vector *this,
			float multiplier)
{
	if (result == NULL || this == NULL)
		return ;
	result->x = this->x * multiplier;
	result->y = this->y * multiplier;
	result->z = this->z * multiplier;
}
