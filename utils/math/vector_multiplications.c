/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_multiplications.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:38:17 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/11 00:03:49 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "vector.h"

t_vector	*vector_new_multiply(t_vector *first, t_vector *second)
{
	t_vector	*ret;

	if (first == NULL || second == NULL)
		return (NULL);
	ret = vector_new(0, 0, 0);
	if (ret == NULL)
		return (NULL);
	vector_multiply(ret, first, second);
	return (ret);
}

t_vector	*vector_new_cross_product(t_vector *first, t_vector *second)
{
	t_vector	*ret;

	if (first == NULL || second == NULL)
		return (NULL);
	ret = vector_new(0, 0, 0);
	if (ret == NULL)
		return (NULL);
	vector_cross_product(ret, first, second);
	return (ret);
}

void	vector_cross_product(
			t_vector *result,
			t_vector *first,
			t_vector *second)
{
	if (result == NULL || first == NULL || second == NULL)
		return ;
	result->x = first->y * second->z - first->z * second->y;
	result->y = first->z * second->x - first->x * second->z;
	result->z = first->x * second->y - first->y * second->x;
}

void	vector_multiply(t_vector *result, t_vector *first, t_vector *second)
{
	if (result == NULL || first == NULL || second == NULL)
		return ;
	result->x = first->x * second->x;
	result->y = first->y * second->y;
	result->z = first->z * second->z;
}
