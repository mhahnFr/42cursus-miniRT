/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 15:42:30 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/31 16:09:55 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	check_sphere(char **splitline)
{
	if (ft_char_arr_len(splitline) < 6)
		return (1);
	if (check_vector(splitline[1]))
		return (1);
	if (check_float(splitline[2]))
		return (1);
	if (ft_atof(splitline[2]) < 0)
		return (1);
	if (check_float(splitline[3]) < 0 || check_float(splitline[3]) > 1)
		return (1);
	if (check_float(splitline[4]) < 0 || check_float(splitline[4]) > 1)
		return (1);
	if (check_rgb(splitline[5]))
		return (1);
	return (0);
}

int	check_plane(char **splitline)
{
	if (ft_char_arr_len(splitline) < 6)
		return (1);
	if (check_vector(splitline[1]))
		return (1);
	if (check_orientation(splitline[2]))
		return (1);
	if (ft_atof(splitline[3]) < 0 || ft_atof(splitline[3]) > 1)
		return (1);
	if (ft_atof(splitline[4]) < 0 || ft_atof(splitline[4]) > 1)
		return (1);
	if (check_rgb(splitline[5]))
		return (1);
	return (0);
}

int	check_cylinder(char **splitline)
{
	if (ft_char_arr_len(splitline) < 8)
		return (1);
	if (check_vector(splitline[1]))
		return (1);
	if (check_orientation(splitline[2]))
		return (1);
	if (check_float(splitline[3]))
		return (1);
	if (ft_atof(splitline[3]) < 0)
		return (1);
	if (check_float(splitline[4]))
		return (1);
	if (ft_atof(splitline[4]) < 0)
		return (1);
	if (ft_atof(splitline[5]) < 0 || ft_atof(splitline[5]) > 1)
		return (1);
	if (ft_atof(splitline[6]) < 0 || ft_atof(splitline[6]) > 1)
		return (1);
	if (check_rgb(splitline[7]))
		return (1);
	return (0);
}
