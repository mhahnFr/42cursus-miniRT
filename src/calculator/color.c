/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 19:19:31 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/14 20:02:13 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <unistd.h>

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

