/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_delete.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:38:12 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/07 16:38:13 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "vector.h"

void	vector_destroy(t_vector __attribute__((unused)) *this)
{
}

void	vector_delete(t_vector *this)
{
	if (this != NULL)
		free(this);
}
