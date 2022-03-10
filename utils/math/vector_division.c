/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_division.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:38:15 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/10 17:55:09 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "vector.h"

t_vector	*vector_new_division(t_vector *divident, float divisor)
{
	t_vector	*ret;

	if (divident == NULL)
		return (NULL);
	ret = vector_new(0, 0, 0);
	if (ret == NULL)
		return (NULL);
	vector_division(ret, divident, divisor);
	return (ret);
}

void	vector_division(
			t_vector *result,
			t_vector *divident,
			float divisor)
{
	if (result == NULL || divident == NULL)
		return ;
	result->x = divident->x / divisor;
	result->y = divident->y / divisor;
	result->z = divident->z / divisor;
}
