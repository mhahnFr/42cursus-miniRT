/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_pars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:01:42 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/13 14:31:11 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "parser.h"
#include "minirt.h"

int	add_light_p2(t_obj_l *curr, char **line)
{
	curr->next->emitter = true;
	curr->next->position = get_vector(line[1]);
	curr->next->brightness = ft_atof(line[2]);
	curr->next->color = get_color(line[3]);
	curr->next->height = 0.5f;
	curr->next->width = curr->next->height;
	if (line[4] != NULL)
		curr->next->intensity = ft_atof(line[4]);
	else
		curr->next->intensity = 20;
	curr->next->obj_type = LIGHT;
	curr->next->next = NULL;
	ft_free_char_arr(line);
	return (0);
}

int	add_light(char **line, t_mixer *data)
{
	t_obj_l	*curr;

	curr = data->obj_list;
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = ft_calloc(1, sizeof(t_obj_l));
	if (curr->next == NULL)
	{
		ft_free_char_arr(line);
		return (3);
	}
	data->light_count++;
	return (add_light_p2(curr, line));
}

int	add_plane(char **line, t_mixer *m_data)
{
	t_obj_l	*curr;

	curr = m_data->obj_list;
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = ft_calloc(1, sizeof(t_obj_l));
	if (curr->next == NULL)
		return (3);
	curr->next->emitter = false;
	curr->next->obj_type = PLANE;
	curr->next->position = get_vector(line[1]);
	curr->next->normal = get_vector(line[2]);
	vector_normalize(&(curr->next->normal));
	curr->next->diffusion = ft_atof(line[3]);
	curr->next->reflec_fac = ft_atof(line[4]);
	curr->next->color = get_color(line[5]);
	curr->next->next = NULL;
	ft_free_char_arr(line);
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
		return (3);
	curr->next->emitter = false;
	curr->next->position = get_vector(line[1]);
	curr->next->height = ft_atof(line[2]);
	curr->next->diffusion = ft_atof(line[3]);
	curr->next->reflec_fac = ft_atof(line[4]);
	curr->next->color = get_color(line[5]);
	curr->next->obj_type = SPHERE;
	curr->next->width = curr->next->height;
	curr->next->next = NULL;
	ft_free_char_arr(line);
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
	curr->next->obj_type = CYLINDER;
	curr->next->next = NULL;
	ft_free_char_arr(line);
	return (0);
}
