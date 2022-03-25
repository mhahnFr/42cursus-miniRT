/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:49:44 by mhahn             #+#    #+#             */
/*   Updated: 2022/03/25 18:18:02 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>

#include "minirt.h"
#include "matrix.h"

void	rt_cam_init(t_mixer *mixer)
{
	//t_matrix	*matrix;
	t_vector	inter;
	size_t		i;
	size_t		j;
	float		angle_x;
	float		angle_y;
	t_vector	u;
	t_vector	v;

	i = 0;
	//matrix = matrix_new(NULL, 3, 3);
	//angle_x = -(mixer->cam.fov / 2);
	//angle_y = (mixer->cam.fov / mixer->cam.aspect_ratio) / 2;
	while (i < RESOLUTION_Y)
	{
		j = 0;
		//angle_y -= (mixer->cam.fov / mixer->cam.aspect_ratio) / RESOLUTION_Y;
		while (j < RESOLUTION_X)
		{
			angle_x = (float) j / RESOLUTION_X;
			angle_y = (float) i / RESOLUTION_Y;
			vector_create(&inter, RESOLUTION_X, 0, 0);
			vector_multiply_digit(&u, &inter, angle_x);
			vector_create(&inter, 0, RESOLUTION_Y, 0);
			vector_multiply_digit(&v, &inter, angle_y);
			vector_addition(&inter, &u, &v);
			vector_create(&u, -(((float )RESOLUTION_X) / 2), -(((float) RESOLUTION_Y) / 2), -(((float) RESOLUTION_Y)/ 2));
			vector_addition(&(mixer->cam.vecs[i][j]), &u, &inter);
			vector_normalize(&(mixer->cam.vecs[i][j]));
			j++;
		}
		i++;
	}
}
