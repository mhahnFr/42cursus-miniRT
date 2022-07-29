/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:49:44 by mhahn             #+#    #+#             */
/*   Updated: 2022/04/05 16:46:41 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>

#include "minirt.h"
#include "matrix.h"

t_vector	rt_cam_svec(t_mixer *mixer, t_vector px, t_vector py, t_vector norm)
{
	t_vector	inter;
	t_vector	s_ret;

	vector_addition(&inter, &px, &py);
	vector_addition(&s_ret, &inter, &norm);
	mixer->cam.step.x = -inter.x * 2 / mixer->res_x;
	mixer->cam.step.y = -inter.y * 2 / mixer->res_y;
	mixer->cam.step.z = 0;
	return (s_ret);
}

t_vector	rt_cam_pxvec(t_mixer *mixer, t_vector norm, t_vector ey)
{
	t_vector	inter;
	t_vector	px_ret;

	vector_cross_product(&inter, &ey, &norm);
	vector_normalize(&inter);
	vector_multiply_digit(&px_ret, &inter, tan(((mixer->cam.fov / 2) * \
	M_PI / 180.0f)));
	return (px_ret);
}

t_vector	rt_cam_pyvec(t_vector norm, t_vector px, t_mixer *mixer)
{
	t_vector	inter;
	t_vector	py_ret;
	float		aspect;

	aspect = (float)((float) mixer->res_y / (float) mixer->res_x);
	vector_cross_product(&inter, &px, &norm);
	vector_normalize(&inter);
	vector_multiply_digit(&py_ret, &inter, vector_length(&px) * aspect);
	return (py_ret);
}

void	build_rays(t_mixer *mixer, t_vector s)
{
	int32_t		i;
	int32_t		ii;

	mixer->cam.vecs = ft_gc_malloc(mixer->res_y * sizeof(t_vector *));
	i = 0;
	while (i < mixer->res_y)
	{
		mixer->cam.vecs[i] = ft_gc_malloc(mixer->res_x * sizeof(t_vector));
		ii = 0;
		while (ii < mixer->res_x)
		{
			mixer->cam.vecs[i][ii].x = s.x + (mixer->cam.step.x * (float) ii);
			mixer->cam.vecs[i][ii].y = s.y + (mixer->cam.step.y * (float) i);
			mixer->cam.vecs[i][ii].z = s.z;
			vector_normalize(&mixer->cam.vecs[i][ii]);
			ii++;
		}
		i++;
	}
}

void	rt_cam(t_mixer *mixer)
{
	t_vector	norm;
	t_vector	px;
	t_vector	py;
	t_vector	s;

	norm = mixer->cam.normal;
	vector_normalize(&norm);
	px = rt_cam_pxvec(mixer, norm, mixer->cam.ey);
	py = rt_cam_pyvec(norm, px, mixer);
	s = rt_cam_svec(mixer, px, py, norm);
	build_rays(mixer, s);
}
