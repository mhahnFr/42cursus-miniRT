/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:49:44 by mhahn             #+#    #+#             */
/*   Updated: 2022/03/27 21:39:42 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>

#include "minirt.h"
#include "matrix.h"

void	rt_cam_init(t_mixer *mixer)
{
	float		alpha;
	float		h_width;
	float		h_height;
	t_vector	inter;
	t_vector	inter2;
	size_t		i;
	size_t		ii;
	t_vector	w;
	t_vector	u;
	t_vector	v;
	t_vector	nup;
	t_vector	hori;
	t_vector	vert;

	//i = 0;
	//alpha = (float) mixer->cam.fov * M_PI / 180;
	////h_width = (float) RESOLUTION_X;
	float aspect = (float) RESOLUTION_X / RESOLUTION_Y;
	//h_height =  aspect * h_width;
	//vector_addition(&w, &mixer->cam.position, &mixer->cam.normal);
	//vector_create(&nup, 0, 1, 0);
	//vector_cross_product(&u, &nup, &w);
	//vector_normalize(&u);
	//vector_cross_product(&v, &w, &u);
	//t_vector lower_left_corner;
	////vector_create(&lower_left_corner, -h_width, -h_height, -1.0);
	//vector_multiply_digit(&inter, &u, h_width);
	//vector_multiply_digit(&inter2, &v, h_height);
	//vector_substract(&inter, &mixer->cam.position, &inter);
	//vector_substract(&inter2, &inter, &inter2);
	//vector_substract(&lower_left_corner, &inter2, &w);
	//vector_multiply_digit(&hori, &u, 2 * h_width);
	//vector_multiply_digit(&vert, &v, 2 * h_height);
	//mixer->cam.hori = hori;
	//mixer->cam.vert = vert;
	//mixer->cam.llc = lower_left_corner;
	i = 0;
	while (i < RESOLUTION_Y)
	{
		ii = 0;
		while (ii < RESOLUTION_X)
		{
			mixer->cam.vecs[i][ii].x = (2 * (ii + 0.5) / RESOLUTION_X - 1) * aspect * tan(mixer->cam.fov / 2 * M_PI / 180);
			mixer->cam.vecs[i][ii].y = (1 - 2 * (i + 0.5) / RESOLUTION_Y) * aspect * tan(mixer->cam.fov / 2 * M_PI / 180);
			mixer->cam.vecs[i][ii].z = 0;
			vector_substract(&mixer->cam.vecs[i][ii], &mixer->cam.vecs[i][ii], &mixer->cam.position);
			vector_normalize(&mixer->cam.vecs[i][ii]);
			https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays?url=3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays
			ii++;
		}
		i++;
	}
}
