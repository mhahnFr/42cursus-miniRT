/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_corrector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 13:15:36 by mhahn             #+#    #+#             */
/*   Updated: 2022/04/08 13:30:19 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	correct_cylinder(t_obj_l *self, t_cam *cam)
{
}

void	correct_spheres(t_obj_l *self, t_cam *cam)
{
	t_vector	inter;

	vector_substract(&inter, &cam->position, self->position);
	self->inv_normal = vector_length(&inter) <= self->height;
}

void	correct_planes(t_obj_l *self, t_cam *cam)
{
}

void	correct_normals(t_mixer *self)
{
	t_obj_l	*inter;

	inter = self->obj_list;
	while (inter != NULL)
	{
		if (inter->obj_type == SPHERE)
			correct_spheres(inter, &mixer->cam);
		else if (inter->obj_type == PLANE)
			correct_planes(inter, &mixer->cam);
		else if (inter->obj_type == CYLINDER)
			correct_cylinder(inter, &mixer->cam);
		inter = inter->next;
	}
}
