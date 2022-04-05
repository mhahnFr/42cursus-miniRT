/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculator.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:42:53 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/05 13:18:42 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minirt.h"
#include "renderer_image.h"
#include <math.h>
#include <stdbool.h>

void	draw_point(int x, int y, t_renderer_image *buf, t_rgbof color)
{
	char	*dst;

	dst = buf->raw + (long)x * (buf->depth / 8) + (long)y * buf->line_size;
	*(unsigned int *) dst = (0 << 24) + (color.r << 16) + (color.g << 8)
		+ (color.b << 0);
}

void	calculator(t_mixer *mixer, int *ret)
{
	calc_object_ray(mixer, ret);
}

void	calc_object_ray(t_mixer *mixer, int *ret)
{
	t_rgbof	color;
	(void) ret;
	for (int i = 0; i < RESOLUTION_Y; i++) {
		for (int j = 0; j < RESOLUTION_X; j++) {
			color = calc_antialiasing(mixer, &(mixer->cam.vecs[i][j]), i, j);
			draw_point(j, i, mixer->image, color);
		}
	}
	printf("Finished!\n");
}

t_vector	vector_rand(t_vector self, float j, float i, t_vector step)
{
	self.x += (float) ((float)(arc4random() % 10) / 10) * step.x;
	self.y += (float) ((float)(arc4random() % 10) / 10) * step.y;
	vector_normalize(&self);
	return (self);
}

t_rgbof	calc_antialiasing(t_mixer *mixer, t_vector *cam_vec, int y, int x)
{
	//antialiasing
	int			i;
	t_rgbof		color;
	t_rgbof		add;
	t_vector	inter;
	t_col		color_sum;

	i = ANTI_ALIASING;
	if (mixer->col_sum.fac != NULL)
		color_sum = mixer->col_sum;
	else
	{
		ft_gc_clear();
		exit(1);	
	}
	inter = *cam_vec;
	vector_normalize(&inter);
	mixer->bounces = 0;
	color_sum.l_count = 0;
	color = calc_shader(&(mixer->cam.position), &inter, mixer, &color_sum);
	color.cal_r = color.r;
	color.cal_g = color.g;
	color.cal_b = color.b;
	while (i > 0)
	{
		mixer->bounces = 0;
		color_sum.l_count = 0;
		inter = vector_rand(*cam_vec, x, y, mixer->cam.step);
		add = calc_shader(&(mixer->cam.position), &inter, mixer, &color_sum);
		color = color_add_cal(color, add);
		i--;
	}
	return (color_cal_rgb(color, ANTI_ALIASING + 1));
}
