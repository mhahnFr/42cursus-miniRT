/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:49:44 by mhahn             #+#    #+#             */
/*   Updated: 2022/03/30 11:13:49 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>

#include "minirt.h"
#include "matrix.h"

void	rt_cam_init(t_mixer *mixer)
{
	float		alpha;
	float		aspect = (float) RESOLUTION_Y / RESOLUTION_X;
	size_t		i;
	size_t		ii;
	t_vector	ey;
	t_vector	norm;
	t_vector	cp;
	t_vector	px;
	t_vector	py;
	t_vector	s;

	vector_create(&ey, 0,1,0);
	norm = mixer->cam.normal;
	cp = mixer->cam.position;
	t_vector	right;
	t_vector	left;
	t_vector	inter;

//px
	vector_addition(&right, &cp, &norm);
	vector_normalize(&norm);
	vector_cross_product(&inter, &ey, &norm);
	vector_normalize(&inter);
	vector_multiply_digit(&px, &inter, tan(((mixer->cam.fov / 2) * M_PI/180.0f)));
//py
	vector_cross_product(&inter, &px, &norm);
	vector_normalize(&inter);
	vector_multiply_digit(&py, &inter, vector_length(&px) * aspect);
//s
	vector_addition(&inter, &px, &py);
	vector_addition(&s, &inter, &norm);
//step
	mixer->cam.step.x = -inter.x * 2 / RESOLUTION_X;
	mixer->cam.step.y = -inter.y * 2 / RESOLUTION_Y;
	mixer->cam.step.z = 0;
	i = 0;
	while (i < RESOLUTION_Y)
	{
		ii = 0;
		while (ii < RESOLUTION_X)
		{
			mixer->cam.vecs[i][ii].x = s.x + (mixer->cam.step.x * ii);
			mixer->cam.vecs[i][ii].y = s.y + (mixer->cam.step.y * i);
			mixer->cam.vecs[i][ii].z = s.z;
			//vector_normalize(&mixer->cam.vecs[i][ii]);
			ii++;
		}
		i++;
	}
}
