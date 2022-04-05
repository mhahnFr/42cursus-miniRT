/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 14:32:21 by mhahn             #+#    #+#             */
/*   Updated: 2022/04/05 15:09:48 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#include <math.h>
#include <pthread.h>
#include <unistd.h>

void	rt_runner(t_thread *self)
{
	size_t	i;
	size_t	j;
	size_t	limit_x;
	size_t	limit_y;
	size_t	start_j;

	i = self->index * self->block_size_x;
	j = self->index * self->block_size_y;
	start_j = j;
	limit_x = i + self->block_size_x;
	limit_y = i + self->block_size_y;
	while (i < limit_x)
	{
		j = start_j;
		while (j < limit_y)
		{
			self->mixer->cam.vecs[j][i].x = s.x + (self->mixer->cam.step.x * i);
			self->mixer->cam.vecs[j][i].y = s.y + (self->mixer->cam.step.y * j);
			self->mixer->cam.vecs[j][i].z = s.z;
			j++;
		}
		i++;
	}
}

void	rt_forker(t_mixer *mixer)
{
	size_t		i;
	pthread_t	*threads;

	mixer->cores = sysconf(_SC_NPROCESSORS_CONF);
	mixer->threads = malloc(sizeof(pthread_t) * cores);
	i = 0;
	while (i < mixer->cores)
	{
		// TODO Error handling!!!
		mixer->threads[i]->index = i;
		mixer->threads[i]->block_size_x = RESOLUTION_X / mixer->cores;
		mixer->threads[i]->block_size_y = RESOLUTION_Y / mixer->cores;
		pthread_create(&mixer->threads[i], NULL, rt_runner, mixer->threads[i]);
		i++;
	}
	i = 0;
	while (i < mixer->cores)
	{
		pthread_join(mixer->threads[i], NULL);
		i++;
	}
}

void	rt_start(t_mixer *mixer)
{
	t_vector	norm;
	t_vector	ey;
	t_vector	cp;
	t_vector	inter;
	t_vector	px;
	t_vector	py;
	t_vector	s;
	t_vector	right;
	float		aspect;

	aspect = (float) RESOLUTION_Y / RESOLUTION_X;
	vector_create(&ey, 0,1,0);
	norm = mixer->cam.normal;
	cp = mixer->cam.position;
	vector_addition(&right, &cp, &norm);
	vector_normalize(&norm);
	vector_cross_product(&inter, &ey, &norm);
	vector_normalize(&inter);
	vector_multiply_digit(&px, &inter, tan(((mixer->cam.fov / 2) * M_PI/180.0f)));
	vector_cross_product(&inter, &px, &norm);
	vector_normalize(&inter);
	vector_multiply_digit(&py, &inter, vector_length(&px) * aspect);
	vector_addition(&inter, &px, &py);
	vector_addition(&s, &inter, &norm);
	mixer->cam.step.x = -inter.x * 2 / RESOLUTION_X;
	mixer->cam.step.y = -inter.y * 2 / RESOLUTION_Y;
	mixer->cam.step.z = 0;
	rt_forker(mixer);
}
