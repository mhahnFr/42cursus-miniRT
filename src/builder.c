/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:49:44 by mhahn             #+#    #+#             */
/*   Updated: 2022/03/25 19:39:46 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>

#include "minirt.h"
#include "matrix.h"

void	rt_cam_init(t_mixer *mixer)
{
	//t_matrix	*matrix;
	float		alpha;
	float		h_width;
	float		h_height;
	

	t_vector	inter;
	t_vector	inter2;
	size_t		i;
	size_t		j;
	float		angle_x;
	float		angle_y;
	t_vector	w;
	t_vector	u;
	t_vector	v;
	t_vector	nup;
	t_vector	llc;
	t_vector	llc_inter;
	t_vector	llc_inter2;
	t_vector	hori;
	t_vector	vert;

	i = 0;
	alpha = mixer->cam.fov * M_PI / 180;
	h_width = tan(alpha / 2);
	h_height = (RESOLUTION_Y / RESOLUTION_X) * h_width;
	vector_substract(&w, &mixer->cam.position, &mixer->cam.normal);
	vector_normalize(&w);
	vector_create(&nup, 0, 1, 0);
	vector_cross_product(&u, &nup, &w);
	vector_normalize(&u);
	vector_cross_product(&v, &w, &u);
	vector_multiply_digit(&llc_inter, &u, h_width);
	vector_multiply_digit(&llc_inter2, &v, h_height);
	vector_substract(&llc_inter2, &llc_inter2, &w);
	vector_substract(&llc_inter, &llc_inter, &llc_inter2);
	vector_substract(&llc, &mixer->cam.position, &llc_inter);
	vector_multiply_digit(&hori, &u, 2 * h_width);
	vector_multiply_digit(&vert, &v, 2 * h_height);
	while (i < RESOLUTION_Y)
	{
		j = 0;
		//angle_y -= (mixer->cam.fov / mixer->cam.aspect_ratio) / RESOLUTION_Y;
		while (j < RESOLUTION_X)
		{
			//angle_x = (float) j / RESOLUTION_X;
			//angle_y = (float) i / RESOLUTION_Y;
			vector_multiply_digit(&inter, &hori, j);
			vector_addition(&inter, &inter, &llc);
			vector_multiply_digit(&inter2, &vert, i);
			vector_addition(&inter, &inter, &inter2);
			vector_substract(&(mixer->cam.vecs[i][j]), &inter, &mixer->cam.position);
			//vector_create(&inter, RESOLUTION_X, 0, 0);
			//vector_multiply_digit(&u, &inter, angle_x);
			//vector_create(&inter, 0, RESOLUTION_Y, 0);
			//vector_multiply_digit(&v, &inter, angle_y);
			//vector_addition(&inter, &u, &v);
			//vector_create(&u, -(((float )RESOLUTION_X) / 2), -(((float) RESOLUTION_Y) / 2), -(((float) RESOLUTION_Y)/ 2));
			vector_normalize(&(mixer->cam.vecs[i][j]));
			j++;
		}
		i++;
	}
}
