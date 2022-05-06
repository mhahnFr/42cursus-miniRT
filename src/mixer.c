/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mixer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:51:25 by mhahn             #+#    #+#             */
/*   Updated: 2022/04/08 17:29:59 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "minirt.h"
#include "mlx.h"
#include "renderer_image.h"

inline void	print_help(void)
{
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

static inline bool	string_equals(const char *str1, const char *str2)
{
	if (ft_strlen(str1) != ft_strlen(str2))
		return (false);
	return (ft_strncmp(str1, str2, ft_strlen(str1)) == 0);
}

static inline bool	string_is_digits(const char *str)
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

static inline void	print_error(const char *arg)
{
	printf("Argument error! Argument \"%s\" ignored\n", arg);
}

static inline void	set_res(t_mixer *self, char *width, char *height)
{
	long	tmp_x;
	long	tmp_y;

	if (!string_is_digits(width) || !string_is_digits(height))
	{
		print_error("--resolution");
		return ;
	}
	tmp_x = ft_atol(width);
	tmp_y = ft_atol(height);
	if (tmp_x <= 0 || tmp_y <= 0)
		print_error("--resolution");
	else
	{
		self->res_x = tmp_x;
		self->res_y = tmp_y;
	}
}

static inline void	set_max_bounces(t_mixer *self, char *bounces)
{
	int	bounce;

	if (!string_is_digits(bounces))
	{
		print_error("--max_bounces");
		return ;
	}
	bounce = ft_atoi(bounces);
	if (bounce < 0)
		print_error("--max_bounces");
	else
		self->max_bounces = bounce;
}

static inline void	set_antialiasing(t_mixer *self, char *factor)
{
	long	fac;

	if (!string_is_digits(factor))
	{
		print_error("--antialiasing");
		return ;
	}
	fac = ft_atol(factor);
	if (fac < 0)
		print_error("--antialiasing");
	else
		self->antialiasing = fac;
}

static inline void	print_values(t_mixer *self)
{
	printf("The current rendering configuration:\n");
	printf("Resolution:   %zu x %zu\n", self->res_x, self->res_y);
	printf("Max bounces:  %d\n", self->max_bounces);
	printf("Antialiasing: %zu\n\n", self->antialiasing);
}

static inline int	config_mixer_loop(t_mixer *self,
										char **argv,
										int argc,
										int *i)
{
	if (string_equals(argv[*i], "-r")
		|| string_equals(argv[*i], "--resolution"))
	{
		if (*i + 2 >= argc)
			print_error("--resolution");
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
			print_error("--max_bounces");
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
			print_error("--antialiasing");
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
					print_error("--file");
				else
					ret = argv[i];
			}
			else
				print_help();
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
	if (argc > 2)
	{
		f = config_mixer_(self, argv, argc);
		if (f == NULL)
			*ret = 21;
		return (f);
	}
	else if (argc == 2 && (string_equals(argv[1], "-h")
			|| string_equals(argv[1], "--help")))
		print_help();
	else if (argc == 2)
		return (argv[1]);
	*ret = 21;
	return (NULL);
}

void	init_mixer_image(t_mixer *self)
{
	self->cam.aspect_ratio = (float) self->res_x / (float) self->res_y;
	self->image
		= renderer_image_new(self->p_mlx_init, self->res_x, self->res_y);
}

t_mixer	*init_mainstruct(int *err)
{
	t_mixer	*ret;

	ret = ft_gc_malloc(sizeof(struct s_mixer));
	if (ret == NULL)
	{
		*err = 3;
		return (NULL);
	}
	ret->p_mlx_init = mlx_init();
	return (ret);
}
