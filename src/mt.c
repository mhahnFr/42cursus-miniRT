/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 14:32:21 by mhahn             #+#    #+#             */
/*   Updated: 2022/04/06 12:30:49 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void	render_ray(t_thread *self, t_vector *ray, size_t x, size_t y)
{
	t_rgbof	color;

	color = calc_antialiasing(self, ray);
	draw_point(x, y, self->mixer->image, color);
}

void	rt_runner(t_thread *self)
{
	size_t	i;
	size_t	j;
	size_t	limit_x;

	limit_x = self->block_size_x * self->index + self->block_size_x;
	if (self->index == self->mixer->cores - 1)
		limit_x = RESOLUTION_X;
	self->col_sum.sum = ft_calloc(1, (self->mixer->light_count + 2) * sizeof(t_vector));
	self->col_sum.fac = ft_calloc(1, (self->mixer->light_count + 2) * sizeof(float));
	i = 0;
	while (i < RESOLUTION_Y)
	{
		j = self->block_size_x * self->index;
		while (j < limit_x)
		{
			render_ray(self, &self->mixer->cam.vecs[i][j], j, i);
			j++;
		}
		i++;
	}
}

void	rt_forker(t_mixer *mixer)
{
	size_t		i;

	mixer->cores = sysconf(_SC_NPROCESSORS_CONF);
	mixer->threads = malloc(sizeof(t_thread) * mixer->cores);
	printf("Using %zu threads to render the scene...\n", mixer->cores);
	i = 0;
	while (i < mixer->cores)
	{
		mixer->threads[i].index = i;
		mixer->threads[i].block_size_x = RESOLUTION_X / mixer->cores;
		mixer->threads[i].mixer = mixer;
		if (pthread_create(&mixer->threads[i].thread, NULL, rt_runner, (void *) &mixer->threads[i]) != 0)
			rt_runner(&mixer->threads[i]);
		i++;
	}
	i = 0;
	while (i < mixer->cores)
	{
		pthread_join(mixer->threads[i].thread, NULL);
		i++;
	}
	printf("Done.\n");
}

void	rt_start(t_mixer *mixer)
{
	rt_forker(mixer);
}
