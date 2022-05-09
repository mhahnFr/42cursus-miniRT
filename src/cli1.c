/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   <Not available>                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1969/12/31 19:51:25 by marvin            #+#    #+#             */
/*   Updated: 1970/01/01 17:29:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline int	config_mixer_loop(t_mixer *self,
										char **argv,
										int argc,
										int *i)
{
	if (string_equals(argv[*i], "-r")
		|| string_equals(argv[*i], "--resolution"))
	{
		if (*i + 2 >= argc)
			print_error_cli("--resolution");
		else
		{
			set_res(self, argv[*i + 1], argv[*i + 2]);
			*i += 2;
		}
		return (true);
	}
	else if (string_equals(argv[*i], "-m")
		|| string_equals(argv[*i], "--max_bounces"))
	{
		*i += 1;
		if (*i >= argc)
			print_error_cli("--max_bounces");
		else
			set_max_bounces(self, argv[*i]);
		return (true);
	}
	return (false);
}

static inline bool	config_mixer_loop2(t_mixer *self,
										char **argv,
										int argc,
										int *i)
{
	if (string_equals(argv[*i], "-a")
		|| string_equals(argv[*i], "--antialiasing"))
	{
		*i += 1;
		if (*i >= argc)
			print_error_cli("--antialiasing");
		else
			set_antialiasing(self, argv[*i]);
		return (true);
	}
	else if (string_equals(argv[*i], "-v")
		|| string_equals(argv[*i], "--values"))
	{
		print_values(self);
		return (true);
	}
	return (false);
}

static inline char	*config_mixer_(t_mixer *self, char **argv, int argc)
{
	int		i;
	char	*ret;

	i = 1;
	ret = NULL;
	while (i < argc)
	{
		if (!config_mixer_loop(self, argv, argc, &i)
			&& !config_mixer_loop2(self, argv, argc, &i))
		{
			if (string_equals(argv[i], "-f")
				|| string_equals(argv[i], "--file"))
			{
				if (++i >= argc)
					print_error_cli("--file");
				else
					ret = argv[i];
			}
			else
				print_help(argv[i]);
		}
		i++;
	}
	return (ret);
}

char	*config_mixer(t_mixer *self, char **argv, int argc, int *ret)
{
	char	*f;

	self->res_x = 1920;
	self->res_y = 1080;
	self->antialiasing = 20;
	self->max_bounces = 30;
	*ret = 0;
	if (argc == 2 && (string_equals(argv[1], "-h")
			|| string_equals(argv[1], "--help")))
	{
		print_help(NULL);
		*ret = 2000;
	}
	else if (argc == 2)
		return (argv[1]);
	else if (argc >= 2)
	{
		f = config_mixer_(self, argv, argc);
		if (f == NULL)
			*ret = 21;
		return (f);
	}
	else
		*ret = 21;
	return (NULL);
}
