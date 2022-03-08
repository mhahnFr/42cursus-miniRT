/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 16:39:55 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/08 20:50:40 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "libft.h"

int	check_float(char *str)
{
	int	i;
	int	sw;

	i = 0;
	sw = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] != '\0' || str[i] != '\n')
	{
		if (str[i] == '.')
			sw++;
		else if (!(str[i] >= '0' || str[i] >= '9'))
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
	while (str[i] != '\0' || str[i] != '\n' || str[i] != ',')
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

int	check_vector(char *str)
{
	int i;

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
	int i;

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

int	check_cam_int(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\n' || str[i] != '\0')
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (1);
		i++;
	}
	if (ft_atoi(str) < 0 || ft_atoi(str) > 180)
		return (1);
	return (0);
}

int	check_rgb_int(char *str, int *count)
{
	int	i;

	i = *count;
	while (str[i] != '\n' || str[i] != '\0' || str[i] != ',')
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

int	check_rgb(char *str)
{
	int i;

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

int	check_camera(char **splitline)
{
	if (ft_char_arr_len(splitline) != 4)
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
	if (ft_char_arr_len(splitline) != 3)
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
	if (ft_char_arr_len(splitline) != 4)
		return (1);
	if (check_vector(splitline[1]))
		return (1);
	if (check_float(splitline[2]))
		return (1);
	if (ft_atof(splitline[2]) < 0 || ft_atof(splitline[2]) > 1)
		return (1);
	if (check_rgb(splitline[3]))
		return (1);
	return (0);
}
int	check_sphere(char **splitline)
{
	if (ft_char_arr_len(splitline) != 4)
		return (1);
	if (check_vector(splitline[1]))
		return (1);
	if (check_float(splitline[2]))
		return (1);
	if (ft_atof(splitline[2]) < 0)
		return (1);
	if (check_rgb(splitline[3]))
		return (1);
	return (0);
}
int	check_plane(char **splitline)
{
	if (ft_char_arr_len(splitline) != 4)
		return (1);
	if (check_vector(splitline[1]))
		return (1);
	if (check_orientation(splitline[2]))
		return (1);
	if (check_rgb(splitline[3]))
		return (1);
	return (0);
}

int	check_cylinder(char **splitline)
{
	if (ft_char_arr_len(splitline) != 6)
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
	if (check_rgb(splitline[5]))
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
		splitline = ft_strsplit(buffer[i], " 	");
		if (!ft_strncmp(splitline[0], "C", 2))				//camera
			ret = check_camera(splitline);
		else if (!ft_strncmp(splitline[0], "A", 2))			//ambient light
			ret = check_ambient(splitline);
		else if (!ft_strncmp(splitline[0], "L", 2))			//light source
			ret = check_light(splitline);
		else if (!ft_strncmp(splitline[0], "sp", 3))		//sphere
			ret = check_sphere(splitline);
		else if (!ft_strncmp(splitline[0], "pl", 3))		//plane
			ret = check_plane(splitline);
		else if (!ft_strncmp(splitline[0], "cy", 3))		//cylinder
			ret = check_cylinder(splitline);
		else
			ret = 1;
		i++;
		//free splitline
	}
	return ret;
}
	/*
	expect token: identifier 							(string) A C L sp pl cy
	expect token: seperator 							(spaces or tabs)
	expect token: position EXCEPTION Ambient light 		(double ',' double ',' double) (float)
	expect token: seperator
	expect token: rotation/diameter/brightness 			(double ',' double ',' double) (float)
	expect token: seperator
	expect token: height/FOV/Color						(double ',' double ',' double) (int) (double ',' double ',' double)
	expect token: seperator
	expect token: Color									(double ',' double ',' double)
	expect token: END OF LINE							(end of string/\n)
	*/
