/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_destroy.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:37:51 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/08 17:35:35 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

void	matrix_destroy(t_matrix __attribute__((unused)) *this)
{
}

void	matrix_delete(t_matrix *this)
{
	unsigned int	i;

	matrix_destroy(this);
	if (this != NULL)
	{
		if (this->values != NULL)
		{
			i = 0;
			while (i < this->rows)
			{
				ft_gc_free(this->values[i]);
				i++;
			}
			ft_gc_free(this->values);
		}
		ft_gc_free(this);
	}
}
