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

static inline void	print_help(void)
{
	printf("Usage: miniRT <file> [-h | -r <width> <height> "
		"| -m <bounces> | -a <factor>]\n");
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
		"the render configuration\n\n");
}

static inline void	set_res(t_mixer *self, char *width, char *height)
{
	// TODO Checks
	self->res_x = ft_atol(width);
	self->res_y = ft_atol(height);
}

static inline void	set_max_bounces(t_mixer *self, char *bounces)
{
	// TODO Checks
	self->max_bounces = ft_atoi(bounces);
}

static inline void	set_antialiasing(t_mixer *self, char *factor)
{
	// TODO Checks
	self->antialiasing = ft_atol(factor);
}

static inline void	print_error(void)
{
	printf("Argument error! Argument ignored");
}

static inline void	print_values(t_mixer *self)
{
	printf("The current rendering configuration:\n");
	printf("Resolution:   %zu x %zu\n", self->res_x, self->res_y);
	printf("Max bounces:  %d\n", self->max_bounces);
	printf("Antialiasing: %zu\n\n", self->antialiasing);
}

static inline void	config_mixer_(t_mixer *self, char **argv, int argc)
{
	int	i;

	i = 2;
	while (i < argc)
	{
		if (ft_strncmp(argv[i], "-r", ft_strlen(argv[i])) == 0
			|| ft_strncmp(argv[i], "--resolution", ft_strlen(argv[i])) == 0)
		{
			if (i + 2 > argc)
				print_error();
			else
			{
				set_res(self, argv[i + 1], argv[i + 2]);
				i += 2;
			}
		}
		else if (ft_strncmp(argv[i], "-m", ft_strlen(argv[i])) == 0
			|| ft_strncmp(argv[i], "--max_bounces", ft_strlen(argv[i])) == 0)
		{
			if (++i > argc)
				print_error();
			else
				set_max_bounces(self, argv[i]);
		}
		else if (ft_strncmp(argv[i], "-a", ft_strlen(argv[i])) == 0
			|| ft_strncmp(argv[i], "--antialiasing", ft_strlen(argv[i])) == 0)
		{
			if (++i > argc)
				print_error();
			else
				set_antialiasing(self, argv[++i]);
		}
		else if (ft_strncmp(argv[i], "-v", ft_strlen(argv[i])) == 0
			|| ft_strncmp(argv[i], "--values", ft_strlen(argv[i])) == 0)
			print_values(self);
		else
			print_help();
		i++;
	}
}

void	config_mixer(t_mixer *self, char **argv, int argc)
{
	self->res_x = 1920;
	self->res_y = 1080;
	self->antialiasing = 20;
	self->max_bounces = 30;
	if (argc > 2)
		config_mixer_(self, argv, argc);
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
