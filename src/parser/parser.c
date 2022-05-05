/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:32:14 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/13 14:56:47 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include "libft.h"
#include "parser.h"
#include "minirt.h"

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
	if (ft_strnstr(buffer, "cu", ft_strlen(buffer)))
		add_cube(ft_strsplit(buffer, " 	"), m_data);
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
		if (ft_strncmp(buffer[i], "#", 1) && ft_strnstr(buffer[i], search, \
		ft_strlen(buffer[i])))
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

static inline int	parser2(char **buffer, t_mixer *m_data)
{
	ft_gc_free(buffer);
	if (m_data->light_count == 0)
		return (5);
	m_data->col_sum.sum = ft_calloc(1, (m_data->light_count + 2) * \
	sizeof(t_vector));
	if (m_data->col_sum.sum == NULL)
		ft_gc_exit(1);
	m_data->col_sum.fac = ft_calloc(1, (m_data->light_count + 2) * \
	sizeof(float));
	if (m_data->col_sum.fac == NULL)
		ft_gc_exit(1);
	return (0);
}

int	parser(char **buffer, t_mixer *m_data, int size)
{
	int	i;

	m_data->light_count = 0;
	if (add_ambient(find_line(buffer, size, "A"), &(m_data->ambient)))
		return (5);
	if (add_camera(find_line(buffer, size, "C"), &(m_data->cam)))
		return (5);
	m_data->obj_list = ft_calloc(1, sizeof(t_obj_l));
	if (m_data->obj_list == NULL)
		return (3);
	m_data->obj_list->obj_type = START;
	i = 0;
	while (buffer[i] != NULL)
	{
		if (ft_strncmp(buffer[i], "#", 1))
			add_object(buffer[i], m_data);
		ft_gc_free(buffer[i]);
		i++;
	}
	return (parser2(buffer, m_data));
}
