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
#include <sys/time.h>
#include "minirt.h"
#ifdef LINUX
#include <bsd/stdlib.h>
#endif
#include "renderer_image.h"

void	draw_point(size_t x, size_t y, t_renderer_image *buf, t_rgbof color)
{
//	u_int8_t	*dst;
//
//	dst = buf->raw + (long)x * (buf->depth / 8) + (long)y * buf->line_size;
//	*(unsigned int *) dst = (0 << 24) + (color.r << 16) + (color.g << 8)
//		+ (color.b << 0);
	mlx_put_pixel(buf->mlx_img, x, y, (color.r << 24) + (color.g << 16)
									  + (color.b << 8) + 255);
}

void	calculator(t_mixer *mixer, int *ret)
{
	t_rgbof	color;
	int32_t	i;
	int32_t	ii;

	(void) ret;
	i = 0;
	printf("00.00 %%\033[?25l");
	while (i < mixer->res_y)
	{
		ii = 0;
		while (ii < mixer->res_x)
		{
			color = calc_first_ray_of_the_day(mixer,
					&(mixer->cam.vecs[mixer->res_y - i - 1][ii]));
			draw_point(ii, i, mixer->image, color);
			ii++;
			printf("\r\r\r\r\r\r\r%.2f %%", (float)((float) \
			(ii + i * mixer->res_x) / (mixer->res_x * mixer->res_y)) *100);
		}
		i++;
	}
	printf("\nFinished!\n\033[?25h");
}

t_vector	vector_rand(t_vector self, t_vector step)
{
	#ifdef WINDOWS
	struct timeval start;
	gettimeofday(&start, NULL);
	srand(start.tv_sec);
	self.x += (float)(((float)(rand() % 10) / 10) * step.x);
	self.y += (float)(((float)(rand() % 10) / 10) * step.y);
	#else
	self.x += (float)(((float)(arc4random() % 10) / 10) * step.x);
	self.y += (float)(((float)(arc4random() % 10) / 10) * step.y);
	#endif
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
	if (mixer->antialiasing > 0)
		return (calc_antialiasing(mixer, cam_vec, color));
	return (color);
}

t_rgbof	calc_antialiasing(t_mixer *mixer, t_vector *cam_vec, t_rgbof color)
{
	size_t		i;
	t_rgbof		add;
	t_vector	inter;
	t_col		color_sum;

	i = mixer->antialiasing;
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
	return (color_cal_rgb(color, mixer->antialiasing + 1));
}
