/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:32:14 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/04 10:36:05 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "vector.h"
#include "libft.h"
#include "minirt.h"

t_rgbof	get_color(char *src)
{
	t_rgbof	color;
	int		i;

	color.r = ft_atoi(src);
	i = 0;
	while (src[i] != ',')
		i++;
	color.g = ft_atoi(src + ++i);
	while (src[i] != ',')
		i++;
	color.b = ft_atoi(src + ++i);
	while (src[i] != ',')
	{
		if (src[i] == '\n' || src[i] == '\0')
			return (color);
		i++;
	}
	color.o = ft_atoi(src + ++i);
	while (src[++i] != ',')
	{
		if (src[i] == '\n' || src[i] == '\0')
			return (color);
		i++; 
	}
	color.f = ft_atoi(src + ++i);
	return (color);
}

t_vector	get_vector(char *src)
{
	t_vector	vec;
	int			i;

	i = 0;
	vec.x = ft_atof(src);
	while (src[i] != ',')
		i++;
	vec.y = ft_atof(src + ++i);
	while (src[i] != ',')
		i++;
	vec.z = ft_atof(src + ++i);
	return (vec);
}

int	add_ambient(char **line, t_ambient *ambient)
{
	if (line == NULL)
		return (5);
	ambient->a_light = ft_atof(line[1]);
	ambient->color = get_color(line[2]);
	ambient->color.cal_r = ambient->color.r;
	ambient->color.cal_g = ambient->color.g;
	ambient->color.cal_b = ambient->color.b;
	ft_free_char_arr(line);
	return (0);
}

int	add_camera(char **line, t_cam *camera)
{
	if (line == NULL)
		return (5);
	camera->position = get_vector(line[1]);
	camera->position.y *= -1;
	camera->normal = get_vector(line[2]);
	vector_normalize(&(camera->normal));
	camera->fov = ft_atoi(line[3]);
	camera->aspect_ratio = RESOLUTION_X / RESOLUTION_Y;
	ft_free_char_arr(line);
	return (0);
}

int	add_light(char **line, t_mixer *data)
{
	t_obj_l	*curr;

	curr = data->obj_list;
	while(curr->next != NULL)
		curr = curr->next;
	curr->next = ft_calloc(1, sizeof(t_obj_l));
	if (curr->next == NULL)
	{
		ft_free_char_arr(line);
		return (3);
	}
	data->light_count++;
	curr->next->emitter = true;
	curr->next->position = get_vector(line[1]);
	curr->next->position.y *= -1;
	curr->next->brightness = ft_atof(line[2]);
	curr->next->color = get_color(line[3]);
	curr->next->height = 0.5;
	curr->next->width = curr->next->height;
	if (line[4] != NULL)
		curr->next->intensity = ft_atof(line[4]);
	else
		curr->next->intensity = 20;
	curr->next->obj_type = LIGHT;
	ft_free_char_arr(line);
	curr->next->next = NULL;
	return (0);
}

int	add_plane(char **line, t_mixer *m_data)
{
	t_obj_l	*curr;

	curr = m_data->obj_list;
	while(curr->next != NULL)
		curr = curr->next;
	curr->next = ft_calloc(1, sizeof(t_obj_l));
	if (curr->next == NULL)
		return (3);
	curr->next->emitter = false;
	curr->next->obj_type = PLANE;
	curr->next->position = get_vector(line[1]);
	curr->next->position.y *= -1;
	curr->next->normal = get_vector(line[2]);
	vector_normalize(&(curr->next->normal));
	curr->next->diffusion = ft_atof(line[3]);
	curr->next->reflec_fac = ft_atof(line[4]);
	curr->next->color = get_color(line[5]);
	curr->next->next = NULL;
	return (0);
}

int	add_sphere(char **line, t_mixer *m_data)
{
	t_obj_l	*curr;

	curr = m_data->obj_list;
	while(curr->next != NULL)
		curr = curr->next;
	curr->next = ft_calloc(1, sizeof(t_obj_l));
	if (curr->next == NULL)
		return (3);
	curr->next->emitter = false;
	curr->next->position = get_vector(line[1]);
	curr->next->position.y *= -1;
	curr->next->height = ft_atof(line[2]);
	curr->next->diffusion = ft_atof(line[3]);
	curr->next->reflec_fac = ft_atof(line[4]);
	curr->next->color = get_color(line[5]);
	curr->next->obj_type = SPHERE;
	curr->next->width = curr->next->height;
	curr->next->next = NULL;
	return (0);
}

int	add_cylinder(char **line, t_mixer *m_data)
{
	t_obj_l	*curr;

	curr = m_data->obj_list;
	while(curr->next != NULL)
		curr = curr->next;
	curr->next = ft_calloc(1, sizeof(t_obj_l));
	if (curr->next == NULL)
		return (3);
	curr->next->emitter = false;
	curr->next->position = get_vector(line[1]);
	curr->next->position.y *= -1;
	curr->next->normal = get_vector(line[2]);
	vector_normalize(&(curr->next->normal));
	curr->next->height = ft_atof(line[3]);
	curr->next->width = ft_atof(line[4]);
	curr->next->diffusion = ft_atof(line[5]);
	curr->next->reflec_fac = ft_atof(line[6]);
	curr->next->color = get_color(line[7]);
	curr->next->obj_type = CYLINDER;
	curr->next->next = NULL;
	return (0);	
}

int	add_object(char *buffer, t_mixer *m_data)
{
	if (buffer == NULL)
		return (5);
	if (ft_strnstr(buffer, "A", ft_strlen(buffer)) || \
	ft_strnstr(buffer, "C", ft_strlen(buffer)))
		return (1);
	if (ft_strnstr(buffer, "pl", ft_strlen(buffer)))
		add_plane(ft_strsplit(buffer, " 	"), m_data);
	if (ft_strnstr(buffer, "sp", ft_strlen(buffer)))
		add_sphere(ft_strsplit(buffer, " 	"), m_data);
	if (ft_strnstr(buffer, "cy", ft_strlen(buffer)))
		add_cylinder(ft_strsplit(buffer, " 	"), m_data);
	if (ft_strnstr(buffer, "L", ft_strlen(buffer)))
		add_light(ft_strsplit(buffer, " 	"), m_data);
	return (0);
}

char	**find_line(char **buffer, int size, char *search)
{
	int		i;
	int		amount;

	i = 0;
	amount = -1;
	while (i < size)
	{
		if (ft_strncmp(buffer[i], "#", 1) && ft_strnstr(buffer[i], search, ft_strlen(buffer[i])))
		{
			if (amount != -1)
				return (NULL);
			amount = i;
		}
		i++;
	}
	if (amount == -1)
		return (NULL);
	return (ft_strsplit(buffer[amount], " 	"));
}

int	parser(char **buffer, t_mixer *m_data, int size)
{
	int 	i;
	int		err;

	m_data->light_count = 0;
	err = add_ambient(find_line(buffer, size, "A"), &(m_data->ambient));
	if (err)
		return (err);
	err = add_camera(find_line(buffer, size, "C"), &(m_data->cam));
	if (err)
		return (err);
	m_data->obj_list = ft_calloc(1, sizeof(t_obj_l));
	if (m_data->obj_list == NULL)
		return (3);
	m_data->obj_list->obj_type = START;
	i = 0;
	while (buffer[i] != NULL)
	{
		if (ft_strncmp(buffer[i], "#", 1))
			add_object(buffer[i], m_data);
		i++;
	}
	if (m_data->light_count == 0)
		return (5);
	m_data->col_sum.sum = ft_calloc(1, (m_data->light_count + 2) * sizeof(t_vector));
	m_data->col_sum.fac = ft_calloc(1, (m_data->light_count + 2) * sizeof(float));
	return (0);
}
