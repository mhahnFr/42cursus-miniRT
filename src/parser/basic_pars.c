/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_pars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 15:58:50 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/08 17:14:03 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
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
