/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   <Not available>                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1969/12/31 19:51:25 by marvin            #+#    #+#             */
/*   Updated: 1969/12/31 17:29:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minirt.h"

inline void	print_help(char *bad_arg)
{
	if (bad_arg != NULL)
		printf("Unrecognized option: \"%s\"!\n", bad_arg);
	printf("Usage: miniRT [<file>] | [-h | -r <width> <height> "
		"| -m <bounces> | -a <factor> | -f <file>]\n");
	printf("\n");
	printf("-r <width> <height>\n");
	printf("--resolution <width> <height>    Specifies the resolution"
		" for the window\n");
	printf("\n");
	printf("-m <bounces>\n");
	printf("--max_bounces <bounces>          Specifies the recursion depth\n");
	printf("\n");
	printf("-a <factor>\n");
	printf("--antialiasing <factor>          Specifies the factor of "
		"antialiasing\n");
	printf("-v\n");
	printf("--values                         Displays "
		"the render configuration\n");
	printf("\n");
	printf("-f\n");
	printf("--file <file>                    Specifies the scene file\n\n");
}

inline bool	string_equals(const char *str1, const char *str2)
{
	if (ft_strlen(str1) != ft_strlen(str2))
		return (false);
	return (ft_strncmp(str1, str2, ft_strlen(str1)) == 0);
}

inline bool	string_is_digits(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

inline void	print_error_cli(const char *arg)
{
	printf("Argument error! Argument \"%s\" ignored\n", arg);
}

inline void	set_res(t_mixer *self, char *width, char *height)
{
	long	tmp_x;
	long	tmp_y;

	if (!string_is_digits(width) || !string_is_digits(height))
	{
		print_error_cli("--resolution");
		return ;
	}
	tmp_x = ft_atol(width);
	tmp_y = ft_atol(height);
	if (tmp_x <= 0 || tmp_y <= 0)
		print_error_cli("--resolution");
	else
	{
		self->res_x = tmp_x;
		self->res_y = tmp_y;
	}
}
