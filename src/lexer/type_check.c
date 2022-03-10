/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 15:38:59 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/10 15:40:31 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	check_float(char *str)
{
	int	i;
	int	sw;

	i = 0;
	sw = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] != '\0' && str[i] != '\n')
	{
		if (str[i] == '.')
			sw++;
		else if (!(str[i] >= '0' && str[i] <= '9'))
			return (1);
		i++;
	}
	if (sw > 1)
		return (1);
	return (0);
}

int	check_vec_float(char *str, int *count)
{
	int	i;
	int	sw;

	i = *count;
	sw = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] != '\0' && str[i] != '\n' && str[i] != ',')
	{
		if (str[i] == '.')
			sw++;
		else if (!(str[i] >= '0' || str[i] >= '9'))
			return (1);
		i++;
	}
	if (sw > 1)
		return (1);
	*count = i;
	return (0);
}

int	check_rgb_int(char *str, int *count)
{
	int	i;

	i = *count;
	while (str[i] != '\n' && str[i] != '\0' && str[i] != ',')
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (1);
		i++;
	}
	if (ft_atoi(str + *count) < 0 || ft_atoi(str + *count) > 255)
		return (1);
	*count = i;
	return (0);
}

int	check_cam_int(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\n' && str[i] != '\0')
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (1);
		i++;
	}
	if (ft_atoi(str) < 0 || ft_atoi(str) > 180)
		return (1);
	return (0);
}
