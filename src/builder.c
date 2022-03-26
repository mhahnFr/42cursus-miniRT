/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:49:44 by mhahn             #+#    #+#             */
/*   Updated: 2022/03/26 01:41:01 by mhahn            ###   ########.fr       */
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
	size_t		j;
	t_vector	w;
	t_vector	u;
	t_vector	v;
	t_vector	nup;
	t_vector	hori;
	t_vector	vert;

	i = 0;
	alpha = (float) mixer->cam.fov * M_PI / 180;
	h_width = (float) tan(alpha / 2);
	float aspect = (float) RESOLUTION_X / RESOLUTION_Y;
	h_height =  aspect * h_width;
	w = mixer->cam.normal;
	vector_create(&nup, 0, 1, 0);
	vector_cross_product(&u, &nup, &w);
	vector_normalize(&u);
	vector_cross_product(&v, &w, &u);
	t_vector lower_left_corner;
	vector_create(&lower_left_corner, -h_width, -h_height, -1.0);
	vector_multiply_digit(&u, &u, h_width);
	vector_multiply_digit(&v, &v, h_height);
	vector_substract(&inter, &mixer->cam.position, &u);
	vector_substract(&inter2, &inter, &v);
	vector_substract(&lower_left_corner, &inter2, &v);
	vector_multiply_digit(&hori, &u, 2 * h_width);
	vector_multiply_digit(&vert, &v, 2 * h_height);
	while (i < RESOLUTION_Y)
	{
		j = 0;
		while (j < RESOLUTION_X)
		{
			vector_multiply_digit(&inter, &hori, j);
			vector_multiply_digit(&inter2, &vert, i);
			vector_addition(&w, &lower_left_corner, &inter);
			vector_addition(&u, &w, &inter2);
			vector_substract(&mixer->cam.vecs[i][j], &u, &mixer->cam.position);
			j++;
		}
		i++;
	}
}
