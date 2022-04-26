/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculator.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:42:53 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/20 14:22:37 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minirt.h"
#include "renderer_image.h"

void	draw_point(int x, int y, t_renderer_image *buf, t_rgbof color)
{
	char	*dst;

	dst = buf->raw + (long)x * (buf->depth / 8) + (long)y * buf->line_size;
	*(unsigned int *) dst = (0 << 24) + (color.r << 16) + (color.g << 8)
		+ (color.b << 0);
}

void	calculator(t_mixer *mixer, int *ret)
{
	t_rgbof	color;
	size_t	i;
	size_t	ii;

	(void) ret;
	i = 0;
	while (i < RESOLUTION_Y)
	{
		ii = 0;
		while (ii < RESOLUTION_X)
		{
			color = calc_first_ray_of_the_day(mixer, &(mixer->cam.vecs[RESOLUTION_Y - i - 1][ii]));
			draw_point(ii, i, mixer->image, color);
			ii++;
		}
		i++;
	}
	printf("Finished!\n");
}

t_vector	vector_rand(t_vector self, t_vector step)
{
	self.x += (float)(((float)(arc4random() % 10) / 10) * step.x);
	self.y += (float)(((float)(arc4random() % 10) / 10) * step.y);
	vector_normalize(&self);
	return (self);
}

t_rgbof	calc_first_ray_of_the_day(t_mixer *mixer, t_vector *cam_vec)
{
	t_col		color_sum;
	t_vector	inter;
	t_rgbof		color;

	if (mixer->col_sum.fac != NULL)
		color_sum = mixer->col_sum;
	else
		ft_gc_exit(1);
	inter = *cam_vec;
	vector_normalize(&inter);
	mixer->bounces = 0;
	color_sum.l_count = 0;
	color = calc_shader(&(mixer->cam.position), &inter, mixer, &color_sum);
	if (ANTI_ALIASING > 0)
		return (calc_antialiasing(mixer, cam_vec, color));
	return (color);
}

t_rgbof	calc_antialiasing(t_mixer *mixer, t_vector *cam_vec, t_rgbof color)
{
	int			i;
	t_rgbof		add;
	t_vector	inter;
	t_col		color_sum;

	i = ANTI_ALIASING;
	color.cal_r = color.r;
	color.cal_g = color.g;
	color.cal_b = color.b;
	color_sum = mixer->col_sum;
	while (i > 0)
	{
		mixer->bounces = 0;
		color_sum.l_count = 0;
		inter = vector_rand(*cam_vec, mixer->cam.step);
		add = calc_shader(&(mixer->cam.position), &inter, mixer, &color_sum);
		color = color_add_cal(color, add);
		i--;
	}
	return (color_cal_rgb(color, ANTI_ALIASING + 1));
}
