/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 14:32:21 by mhahn             #+#    #+#             */
/*   Updated: 2022/04/07 12:20:26 by mhahn            ###   ########.fr       */
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

/* void	rt_runner(t_thread *self)
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
} */

bool	rt_block_fetcher(t_tile **tile_array, t_tile *render)
{
	int	i;
	int	ii;
	int	tiles_per_axis;

	i = 0;
	tiles_per_axis = ceil((double) RESOLUTION_Y / BLOCK_SIZE);
	while (i < tiles_per_axis)
	{
		ii = 0;
		while (ii < tiles_per_axis)
		{
			if (pthread_mutex_trylock(&tile_array[i][ii].m_rendered) == 0)
			{
				if (!tile_array[i][ii].rendered)
				{
					tile_array[i][ii].rendered = true;
					pthread_mutex_unlock(&tile_array[i][ii].m_rendered);
					*render = tile_array[i][ii];
					return (true);
				}
				pthread_mutex_unlock(&tile_array[i][ii].m_rendered);
			}
			ii++;
		}
		i++;
	}
	return (false);
}

void	rt_runner(t_thread *self)
{
	t_tile	to_render;
	int		i;
	int		ii;

	self->col_sum.sum = ft_calloc(1, (self->mixer->light_count + 2) * sizeof(t_vector));
	self->col_sum.fac = ft_calloc(1, (self->mixer->light_count + 2) * sizeof(float));
	while (rt_block_fetcher(self->mixer->tile_array, &to_render))
	{
		i = to_render.x;
		while (i < to_render.x + (ceil((double) BLOCK_SIZE * self->mixer->cam.aspect_ratio)) && i < RESOLUTION_X)
		{
			ii = to_render.y;
			while (ii < to_render.y + BLOCK_SIZE && ii < RESOLUTION_Y)
			{
				render_ray(self, &self->mixer->cam.vecs[ii][i], i, ii);
				ii++;
			}
			i++;
		}
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
		mixer->threads[i].mixer = mixer;
		if (pthread_create(&mixer->threads[i].thread, NULL, rt_runner, (void *) &mixer->threads[i]) != 0)
			;//rt_runner(&mixer->threads[i]);
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

t_tile	**rt_divide(float aspect)
{
	t_tile	**ret;
	int		tiles_per_axis;
	int		i;
	int		ii;

	tiles_per_axis = ceil((double) RESOLUTION_Y / BLOCK_SIZE);
	//ret = malloc((long) pow(tiles_per_axis, 2) * sizeof(t_tile));
	ret = malloc(tiles_per_axis * sizeof(void *));
	i = 0;
	while (i < tiles_per_axis)
	{
		ii = 0;
		ret[i] = malloc(tiles_per_axis * sizeof(t_tile));
		while (ii < tiles_per_axis)
		{
			//ret[i] = (void *) ret + tiles_per_axis * i + ii * sizeof(t_tile) + tiles_per_axis * sizeof(void *);
			ret[i][ii].y = i * BLOCK_SIZE;
			ret[i][ii].x = ii * ceil((double) BLOCK_SIZE * aspect);
			pthread_mutex_init(&ret[i][ii].m_rendered, NULL);
			ret[i][ii].rendered = false;
			ii++;
		}
		i++;
	}
	return (ret);
}

void	rt_start(t_mixer *mixer)
{
	mixer->tile_array = rt_divide(mixer->cam.aspect_ratio);
	rt_forker(mixer);
}
