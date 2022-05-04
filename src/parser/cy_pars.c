/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cy_pars.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 13:54:54 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/20 16:05:32 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "parser.h"
#include <stdbool.h>

void	top_position(t_obj_l *obj)
{
	t_vector	inter;

	vector_multiply_digit(&inter, &obj->normal, obj->height);
	vector_addition(&obj->position, &inter, &obj->position);
}

int	add_cy_caps(char **line, t_mixer *m_data, bool bottom)
{
	t_obj_l	*cur;

	cur = m_data->obj_list;
	while (cur->next != NULL)
		cur = cur->next;
	cur->next = ft_calloc(1, sizeof(t_obj_l));
	if (cur->next == NULL)
		return (3);
	cur->next->emitter = false;
	cur->next->position = get_vector(line[1]);
	cur->next->normal = get_vector(line[2]);
	vector_normalize(&(cur->next->normal));
	cur->next->height = ft_atof(line[3]);
	cur->next->width = ft_atof(line[4]);
	cur->next->diffusion = ft_atof(line[5]);
	cur->next->reflec_fac = ft_atof(line[6]);
	cur->next->color = get_color(line[7]);
	cur->next->obj_type = CYLINDER_CAP;
	if (bottom)
		vector_multiply_digit(&(cur->next->normal), &(cur->next->normal), -1);
	else
		top_position(cur->next);
	cur->next->next = NULL;
	cur->next->prev = cur;
	return (0);
}

int	add_cy_mant(char **line, t_mixer *m_data)
{
	t_obj_l	*curr;

	curr = m_data->obj_list;
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = ft_calloc(1, sizeof(t_obj_l));
	if (curr->next == NULL)
		return (3);
	curr->next->emitter = false;
	curr->next->position = get_vector(line[1]);
	curr->next->normal = get_vector(line[2]);
	vector_normalize(&(curr->next->normal));
	curr->next->height = ft_atof(line[3]);
	curr->next->width = ft_atof(line[4]);
	curr->next->diffusion = ft_atof(line[5]);
	curr->next->reflec_fac = ft_atof(line[6]);
	curr->next->color = get_color(line[7]);
	curr->next->obj_type = CYLINDER_MAN;
	curr->next->next = NULL;
	curr->next->prev = curr;
	return (0);
}
