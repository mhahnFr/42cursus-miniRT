/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 14:32:21 by mhahn             #+#    #+#             */
/*   Updated: 2022/04/05 16:05:50 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#include <math.h>
#include <pthread.h>
#include <unistd.h>

void	render_ray(t_mixer *mixer, t_vector *ray, size_t x, size_t y)
{
	t_rgbof	color;

	color = calc_antialiasing(mixer, ray, y, x);
	draw_point(x, y, mixer->image, color);
}

void	rt_runner(t_thread *self)
{
	size_t	i;
	size_t	j;
	size_t	limit_x;
	size_t	limit_y;
	size_t	start_y;

	i = self->block_size_x * self->index;
	j = self->block_size_y * self->index;
	limit_x = i + self->block_size_x;
	limit_y = j + self->block_size_y;
	start_y = j;
	while (i < limit_x)
	{
		j = start_y;
		while (j < limit_y)
		{
			render_ray(self->mixer, &self->mixer->cam.vecs[j][i], i, j);
			j++;
		}
		i++;
	}
}

void	rt_forker(t_mixer *mixer)
{
	size_t		i;
	pthread_t	*threads;

	//mixer->cores = sysconf(_SC_NPROCESSORS_CONF);
	mixer->cores = 2;
	mixer->threads = malloc(sizeof(t_thread) * mixer->cores);
	i = 0;
	while (i < mixer->cores)
	{
		// TODO Error handling!!!
		mixer->threads[i].index = i;
		mixer->threads[i].block_size_x = RESOLUTION_X / mixer->cores;
		mixer->threads[i].block_size_y = RESOLUTION_Y / mixer->cores;
		mixer->threads[i].mixer = mixer;
		pthread_create(&mixer->threads[i].thread, NULL, rt_runner, (void *) &mixer->threads[i]);
		i++;
	}
	i = 0;
	while (i < mixer->cores)
	{
		pthread_join(mixer->threads[i].thread, NULL);
		i++;
	}
}

void	rt_start(t_mixer *mixer)
{
	rt_forker(mixer);
}
