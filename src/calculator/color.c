/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 19:19:31 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/19 18:40:05 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <unistd.h>
#include <stdio.h>

t_rgbof	color_rgb(int r, int g, int b)
{
	t_rgbof	ret;

	ret.r = r;
	ret.g = g;
	ret.b = b;
	if (r > 255)
		ret.r = 255;
	if (g > 255)
		ret.g = 255;
	if (b > 255)
		ret.b = 255;
	return (ret);
}

t_rgbof	color_add(t_rgbof first, t_rgbof second)
{
	int	cal_r;
	int	cal_g;
	int	cal_b;

	cal_r = first.r + second.r;
	cal_g = first.g + second.g;
	cal_b = first.b + second.b;
	first.r = cal_r;
	first.g = cal_g;
	first.b = cal_b;
	if (cal_r > 255)
		first.r = 255;
	if (cal_g > 255)
		first.g = 255;
	if (cal_b > 255)
		first.b = 255;
	return (first);
}

t_rgbof	color_add_cal_cal(t_rgbof first, t_rgbof second)
{
	first.cal_r += second.cal_r;
	first.cal_g += second.cal_g;
	first.cal_b += second.cal_b;
	return (first);
}

t_rgbof	color_add_cal(t_rgbof first, t_rgbof second)
{
	first.cal_r += second.r;
	first.cal_g += second.g;
	first.cal_b += second.b;
	return (first);
}

void	color_rgb_cal_result_mul(t_rgbof *res, t_rgbof color, float factor)
{
	(*res).cal_r += color.r * factor;
	(*res).cal_g += color.g * factor;
	(*res).cal_b += color.b * factor;
}

t_rgbof	color_cal_rgb(t_rgbof color, float factor)
{
	if (factor > 1)
	{
		color.r = color.cal_r / factor;
		color.g = color.cal_g / factor;
		color.b = color.cal_b / factor;
	} else
	{
		color.r = color.cal_r * factor;
		color.g = color.cal_g * factor;
		color.b = color.cal_b * factor;
	}
	return (color);
}

void	color_print(t_rgbof color)
{
	printf("r: %i	cal: %i\ng: %i	cal: %i\nb: %i	cal: %i\n\n", color.r, color.cal_r, color.g, color.cal_g, color.b, color.cal_b);
}
