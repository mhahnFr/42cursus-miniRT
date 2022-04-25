/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 19:19:31 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/11 14:35:22 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
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

t_rgbof	color_add_cal(t_rgbof first, t_rgbof second)
{
	first.cal_r += second.r;
	first.cal_g += second.g;
	first.cal_b += second.b;
	return (first);
}

t_rgbof	color_cal_rgb(t_rgbof color, float factor)
{
	if (factor > 1)
	{
		color.r = color.cal_r / factor;
		color.g = color.cal_g / factor;
		color.b = color.cal_b / factor;
	}
	else
	{
		color.r = color.cal_r * factor;
		color.g = color.cal_g * factor;
		color.b = color.cal_b * factor;
	}
	return (color);
}
