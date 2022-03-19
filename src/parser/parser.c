/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:32:14 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/19 19:15:35 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
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
	camera->position = get_vector(line[1]);
	camera->normal = get_vector(line[2]);
	vector_normalize(&(camera->normal));
	camera->fov = ft_atoi(line[3]);
	camera->aspect_ratio = RESOLUTION_X / RESOLUTION_Y;
	ft_free_char_arr(line);
	return (0);
}

int	add_light(char **line, t_mixer *data)
{
	data->obj_list = ft_calloc(1, sizeof(t_obj_l));
	if (data->obj_list == NULL)
	{
		ft_free_char_arr(line);
		return (1);
	}
	data->obj_list->emitter = true;
	data->obj_list->position = get_vector(line[1]);
	data->obj_list->brightness = ft_atof(line[2]);
	data->obj_list->color = get_color(line[3]);
	data->obj_list->obj_type = LIGHT;
	ft_free_char_arr(line);
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
		return (1);
	curr->next->emitter = false;
	curr->next->obj_type = PLANE;
	curr->next->position = get_vector(line[1]);
	curr->next->normal = get_vector(line[2]);
	vector_normalize(&(curr->next->normal));
	curr->next->color = get_color(line[3]);
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
		return (1);
	curr->next->emitter = false;
	curr->next->position = get_vector(line[1]);
	curr->next->height = ft_atof(line[2]);
	curr->next->obj_type = SPHERE;
	curr->next->width = curr->height;
	curr->next->color = get_color(line[3]);
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
		return (1);
	curr->next->emitter = false;
	curr->next->position = get_vector(line[1]);
	curr->next->normal = get_vector(line[2]);
	vector_normalize(&(curr->next->normal));
	curr->next->height = ft_atof(line[3]);
	curr->next->width = ft_atof(line[4]);
	curr->next->color = get_color(line[5]);
	curr->next->obj_type = CYLINDER;
	curr->next->next = NULL;
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

char	**find_line(char **buffer, int size, char *search)
{
	int		i;

	i = 0;
	while (i < size)
	{
		if (ft_strnstr(buffer[i], search, ft_strlen(buffer[i])))
			return (ft_strsplit(buffer[i], " 	"));
		i++;
	}
	return (NULL);
}

int	parser(char **buffer, t_mixer *m_data, int size)
{
	int i;

	if (add_ambient(find_line(buffer, size, "A"), &(m_data->ambient)))
		return (1);
	if (add_camera(find_line(buffer, size, "C"), &(m_data->cam)))
		return (1);
	if (add_light(find_line(buffer, size, "L"), m_data))
		return (1);
	i = 0;
	while (buffer[i] != NULL)
	{
		add_object(buffer[i], m_data);
		i++;
	}
	return (0);
}
