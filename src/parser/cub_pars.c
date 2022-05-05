/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_pars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 17:31:23 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/26 17:31:24 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "libft.h"
#include "parser.h"

//side: 0 top, 1 bottom, 2 left, 3 right, 4 front, 5 back
t_vector	determine_position(t_obj_l *self, t_vector vec, int side)
{
	t_vector	inter;

	if (side == 1)
		vector_multiply_digit(&inter, &self->normal, -(self->height / 2));
	else if (side == 0)
		vector_multiply_digit(&inter, &self->normal, self->height / 2);
	else if (side == 2 || side == 3)
		vector_multiply_digit(&inter, &self->normal, (self->width / 2));
	else if (side == 4 || side == 5)
		vector_multiply_digit(&inter, &self->normal, self->depth / 2);
	vector_addition(&vec, &vec, &inter);
	return (vec);
}

void	determine_normal_the_sequel(t_vector up, \
									t_vector front, int side, t_obj_l *self)
{
	t_vector	cross;

	vector_normalize(&up);
	vector_normalize(&front);
	vector_cross_product(&cross, &up, &front);
	vector_normalize(&cross);
	if (side == 2 || side == 3)
	{
		vector_multiply_digit(&self->normal, &cross, -1);
		if (side == 3)
			vector_multiply_digit(&self->normal, &cross, 1);
		self->normal_left = up;
		vector_multiply_digit(&self->normal_down, &front, 1);
	}
	if (side == 4 || side == 5)
	{
		vector_multiply_digit(&self->normal, &front, 1);
		if (side == 5)
			vector_multiply_digit(&self->normal, &front, -1);
		vector_multiply_digit(&self->normal_down, &cross, 1);
		vector_multiply_digit(&self->normal_left, &up, 1);
	}
}

void	determine_normal(t_vector up, t_vector front, int side, t_obj_l *self)
{
	t_vector	cross;

	vector_normalize(&up);
	vector_normalize(&front);
	vector_cross_product(&cross, &up, &front);
	vector_normalize(&cross);
	if (side == 0 || side == 1)
	{
		self->normal = up;
		self->normal_left = cross;
		self->normal_down = front;
	}
	else
		determine_normal_the_sequel(up, front, side, self);
}

//side: 0 top, 1 bottom, 2 left, 3 right, 4 front, 5 back
int	add_cube_plane(char **line, t_mixer *m_data, int side)
{
	t_obj_l	*curr;

	curr = m_data->obj_list;
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = ft_calloc(1, sizeof(t_obj_l));
	if (curr->next == NULL)
		return (3);
	curr->next->prev = curr;
	curr = curr->next;
	curr->emitter = false;
	curr->cube_side = side;
	determine_normal(get_vector(line[2]), get_vector(line[3]), side, curr);
	curr->height = ft_atof(line[4]);
	curr->width = ft_atof(line[5]);
	curr->depth = ft_atof(line[6]);
	curr->position = determine_position(curr, get_vector(line[1]), side);
	curr->diffusion = ft_atof(line[7]);
	curr->reflec_fac = ft_atof(line[8]);
	if (ft_char_arr_len(line) < 15)
		curr->color = get_color(line[9]);
	else
		curr->color = get_color(line[9 + side]);
	curr->obj_type = CUBE_PLANE;
	return (0);
}

int	add_cube(char **line, t_mixer *m_data)
{
	if (add_cube_plane(line, m_data, 0))
		return (3);
	if (add_cube_plane(line, m_data, 1))
		return (3);
	if (add_cube_plane(line, m_data, 2))
		return (3);
	if (add_cube_plane(line, m_data, 3))
		return (3);
	if (add_cube_plane(line, m_data, 4))
		return (3);
	if (add_cube_plane(line, m_data, 5))
		return (3);
	ft_free_char_arr(line);
	return (0);
}
