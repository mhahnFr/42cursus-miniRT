/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_copy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:37:14 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/07 16:37:15 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "point.h"

void	point_copy_values(t_point *this, t_point *copy)
{
	if (this == NULL || copy == NULL)
		return ;
	point_create(copy, this->x, this->y);
	copy->r = this->r;
	copy->g = this->g;
	copy->b = this->b;
}

t_point	*point_copy(t_point *this)
{
	t_point	*ret;

	if (this == NULL)
		return (NULL);
	ret = point_new(this->x, this->y);
	if (ret != NULL)
		point_copy_values(this, ret);
	return (ret);
}
