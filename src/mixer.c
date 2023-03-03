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
#include "MLX42/MLX42.h"
#include "renderer_image.h"

inline void	set_max_bounces(t_mixer *self, char *bounces)
{
	int	bounce;

	if (!string_is_digits(bounces))
	{
		print_error_cli("--max_bounces");
		return ;
	}
	bounce = ft_atoi(bounces);
	if (bounce < 0)
		print_error_cli("--max_bounces");
	else
		self->max_bounces = bounce;
}

inline void	set_antialiasing(t_mixer *self, char *factor)
{
	long	fac;

	if (!string_is_digits(factor))
	{
		print_error_cli("--antialiasing");
		return ;
	}
	fac = ft_atol(factor);
	if (fac < 0)
		print_error_cli("--antialiasing");
	else
		self->antialiasing = fac;
}

inline void	print_values(t_mixer *self)
{
	printf("The current rendering configuration:\n");
	printf("Resolution:   %i x %i\n", self->res_x, self->res_y);
	printf("Max bounces:  %d\n", self->max_bounces);
	printf("Antialiasing: %zu\n\n", self->antialiasing);
}

void	init_mixer_image(t_mixer *self)
{

	self->mlx = mlx_init(self->res_x, self->res_y, "miniRT", false);
	self->cam.aspect_ratio = (float) self->res_x / (float) self->res_y;
	self->image
		= renderer_image_new(self->mlx, self->res_x, self->res_y);
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
//	ret->mlx = mlx_init();
	return (ret);
}
