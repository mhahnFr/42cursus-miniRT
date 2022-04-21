/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_length.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:36:33 by mhahn             #+#    #+#             */
/*   Updated: 2022/03/10 23:59:23 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "vector.h"

float	vector_distance(t_vector *first, t_vector *second)
{
	if (first == NULL || second == NULL)
		return (0);
	return (sqrt(powf(first->x - second->x, 2) + powf(first->y - second->y, 2)
			+ powf(first->z - second->z, 2)));
}
