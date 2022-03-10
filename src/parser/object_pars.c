/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_pars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:01:42 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/10 17:42:54 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minirt.h"

int	add_plane(char **line, t_mixer *m_data)
{
	t_obj_l	*curr;

	curr = m_data->obj_list;
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = ft_calloc(1, sizeof(t_obj_l));
	if (curr->next == NULL)
		return (1);
	curr->next->emitter = false;
	curr->next->position = get_vector(line[1]);
	curr->next->normal = get_vector(line[2]);
	curr->next->color = get_color(line[3]);
	return (0);
}

int	add_sphere(char **line, t_mixer *m_data)
{
	t_obj_l	*curr;

	curr = m_data->obj_list;
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = ft_calloc(1, sizeof(t_obj_l));
	if (curr->next == NULL)
		return (1);
	curr->next->emitter = false;
	curr->next->position = get_vector(line[1]);
	curr->next->height = ft_atof(line[2]);
	curr->next->width = curr->height;
	curr->next->color = get_color(line[3]);
	return (0);
}

int	add_cylinder(char **line, t_mixer *m_data)
{
	t_obj_l	*curr;

	curr = m_data->obj_list;
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = ft_calloc(1, sizeof(t_obj_l));
	if (curr->next == NULL)
		return (1);
	curr->next->emitter = false;
	curr->next->position = get_vector(line[1]);
	curr->next->normal = get_vector(line[2]);
	curr->next->height = ft_atof(line[3]);
	curr->next->width = ft_atof(line[4]);
	curr->next->color = get_color(line[5]);
	return (0);
}

int	add_object(char *buffer, t_mixer *m_data)
{
	if (ft_strnstr(buffer, "A", ft_strlen(buffer)) || \
	ft_strnstr(buffer, "C", ft_strlen(buffer)) || \
	ft_strnstr(buffer, "L", ft_strlen(buffer)))
		return (1);
	if (ft_strnstr(buffer, "pl", ft_strlen(buffer)))
		add_plane(ft_strsplit(buffer, " 	"), m_data);
	if (ft_strnstr(buffer, "sp", ft_strlen(buffer)))
		add_sphere(ft_strsplit(buffer, " 	"), m_data);
	if (ft_strnstr(buffer, "cy", ft_strlen(buffer)))
		add_cylinder(ft_strsplit(buffer, " 	"), m_data);
	return (0);
}
