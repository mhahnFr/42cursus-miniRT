/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_corrector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 13:15:36 by mhahn             #+#    #+#             */
/*   Updated: 2022/04/08 14:41:54 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	correct_cylinder(t_obj_l *self, t_cam *cam)
{
	float		t;
	t_vector	res;

	if (self->obj_type == CYLINDER_MAN)
	{
		self->inv_normal = false;
		vector_substract(&res, &cam->position, &self->position);
		t = vector_scalar_product(&res, &self->normal);
		if (t > self->height)
			return ;
		vector_multiply_digit(&res, &cam->position, t);
		vector_addition(&res, &res, &self->position);
		vector_substract(&res, &cam->position, &res);
		self->inv_normal = vector_length(&res) <= self->width;
	}
	else if (self->obj_type == CYLINDER_CAP)
		self->inv_normal = self->prev->inv_normal;
}

void	correct_spheres(t_obj_l *self, t_cam *cam)
{
	t_vector	inter;

	vector_substract(&inter, &cam->position, &self->position);
	self->inv_normal = vector_length(&inter) <= self->height;
}

void	correct_planes(t_obj_l *self, t_cam *cam)
{
	t_vector	inter;

	vector_substract(&inter, &self->position, &cam->position);
	vector_normalize(&inter);
	vector_addition(&inter, &inter, &self->normal);
	self->inv_normal = vector_length(&inter) < 1;
	if (self->inv_normal)
		vector_multiply_digit(&self->normal, &self->normal, -1);
}

void	correct_normals(t_mixer *self)
{
	t_obj_l	*inter;

	inter = self->obj_list;
	while (inter != NULL)
	{
		if (inter->obj_type == SPHERE)
			correct_spheres(inter, &self->cam);
		else if (inter->obj_type == PLANE)
			correct_planes(inter, &self->cam);
		else if (inter->obj_type >= CYLINDER_CAP)
			correct_cylinder(inter, &self->cam);
		inter = inter->next;
	}
}
