/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 15:36:38 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/10 15:40:12 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	check_vector(char *str)
{
	int	i;

	i = 0;
	if (check_vec_float(str, &i))
		return (1);
	if (str[i++] != ',')
		return (1);
	if (check_vec_float(str, &i))
		return (1);
	if (str[i++] != ',')
		return (1);
	if (check_vec_float(str, &i))
		return (1);
	return (0);
}

int	check_orientation(char *str)
{
	int	i;

	i = 0;
	if (ft_atof(str + i) < 0 || ft_atof(str + i) > 1)
		return (1);
	if (check_vec_float(str, &i))
		return (1);
	if (str[i++] != ',')
		return (1);
	if (ft_atof(str + i) < 0 || ft_atof(str + i) > 1)
		return (1);
	if (check_vec_float(str, &i))
		return (1);
	if (str[i++] != ',')
		return (1);
	if (ft_atof(str + i) < 0 || ft_atof(str + i) > 1)
		return (1);
	if (check_vec_float(str, &i))
		return (1);
	return (0);
}

int	check_rgb(char *str)
{
	int	i;

	i = 0;
	if (check_rgb_int(str, &i))
		return (1);
	if (str[i++] != ',')
		return (1);
	if (check_rgb_int(str, &i))
		return (1);
	if (str[i++] != ',')
		return (1);
	if (check_rgb_int(str, &i))
		return (1);
	return (0);
}
