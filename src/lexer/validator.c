/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 16:39:55 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/31 16:03:14 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdio.h>
#include "libft.h"
#include "lexer.h"

void	p_chararr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
		printf("%s ", arr[i++]);
}

int	check_camera(char **splitline)
{
	if (ft_char_arr_len(splitline) < 4)
		return (1);
	if (check_vector(splitline[1]))
		return (1);
	if (check_orientation(splitline[2]))
		return (1);
	if (check_cam_int(splitline[3]))
		return (1);
	return (0);
}

int	check_ambient(char **splitline)
{
	if (ft_char_arr_len(splitline) < 3)
		return (1);
	if (check_float(splitline[1]))
		return (1);
	if (ft_atof(splitline[1]) < 0 || ft_atof(splitline[1]) > 1)
		return (1);
	if (check_rgb(splitline[2]))
		return (1);
	return (0);
}

int	check_light(char **splitline)
{
	if (ft_char_arr_len(splitline) < 4)
		return (1);
	if (check_vector(splitline[1]))
		return (1);
	if (check_float(splitline[2]))
		return (1);
	if (ft_atof(splitline[2]) < 0 || ft_atof(splitline[2]) > 1)
		return (1);
	if (check_rgb(splitline[3]))
		return (1);
	if (splitline[4] != NULL && check_float(splitline[4]))
		return (1);
	return (0);
}

//I wish I had a switch tonight!
int	validation_check(char **buffer, int size)
{
	int		ret;
	int		i;
	char	**splitline;

	ret = 0;
	i = 0;
	while (i < size && !ret)
	{
		splitline = ft_strsplit(buffer[i++], " 	");
		if (!ft_strncmp(splitline[0], "#", 1))
			ret = 0;
		else if (!ft_strncmp(splitline[0], "C", 2))
			ret = check_camera(splitline);
		else if (!ft_strncmp(splitline[0], "A", 2))
			ret = check_ambient(splitline);
		else if (!ft_strncmp(splitline[0], "L", 2))
			ret = check_light(splitline);
		else if (!ft_strncmp(splitline[0], "sp", 3))
			ret = check_sphere(splitline);
		else if (!ft_strncmp(splitline[0], "pl", 3))
			ret = check_plane(splitline);
		else if (!ft_strncmp(splitline[0], "cy", 3))
			ret = check_cylinder(splitline);
		else
			ret = 1;
	}
	return (ret);
}
