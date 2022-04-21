/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:51:57 by mhahn             #+#    #+#             */
/*   Updated: 2022/04/08 17:35:12 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vertex.h"
#include "matrix.h"

t_vertex3d	*vertex3d_new(float x, float y, float z)
{
	t_vertex3d	*ret;

	ret = ft_gc_malloc(sizeof(struct s_vertex3d));
	if (ret != NULL)
		vertex3d_create(ret, x, y, z);
	return (ret);
}

void	vertex3d_create(t_vertex3d *this, float x, float y, float z)
{
	unsigned int	colour;

	if (this == NULL)
		return ;
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = 1;
	colour = 0x00FFFFFF;
	this->r = colour >> 16;
	this->g = colour >> 8;
	this->b = colour >> 0;
	this->colour_set = false;
}

void	vertex3d_destroy(t_vertex3d __attribute__((unused)) *this)
{
}

void	vertex3d_delete(t_vertex3d *this)
{
	vertex3d_destroy(this);
	if (this != NULL)
		ft_gc_free(this);
}
