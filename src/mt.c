/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 14:32:21 by mhahn             #+#    #+#             */
/*   Updated: 2022/04/11 12:52:44 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef void*(*t_run)(void*);
void draw_point(size_t x, size_t y, t_renderer_image *image, t_rgbof colour);


void	render_ray(t_thread *self, t_vector *ray, size_t x, size_t y)
{
	t_rgbof	color;

	color = calc_first_ray_of_the_day(self->mixer, ray);
	draw_point(x, y, self->mixer->image, color);
}

bool	rt_block_fetcher(t_tile **tile_array, t_tile *render, size_t index, size_t iteration, size_t cores)
{
	//size_t	i;
	//size_t	ii;
	size_t	tiles_per_axis;

	//i = 0;
	tiles_per_axis = ceil((double) RESOLUTION_Y / BLOCK_SIZE);
	/*while (i < tiles_per_axis)
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
	}*/
	size_t next = index + (cores * iteration);
	size_t x = next % tiles_per_axis;
	size_t y = next / tiles_per_axis;
	if (x < tiles_per_axis && y < tiles_per_axis) {
		*render = tile_array[x][next / tiles_per_axis];
		return (true);
	}
	return (false);
}

void	rt_runner(t_thread *self)
{
	t_tile	to_render;
	size_t	i;
	size_t	ii;
	size_t	limit_x;

	self->col_sum.sum = ft_calloc(1, (self->mixer->light_count + 2) * sizeof(t_vector));
	self->col_sum.fac = ft_calloc(1, (self->mixer->light_count + 2) * sizeof(float));
	size_t iteration = 0;
	while (rt_block_fetcher(self->mixer->tile_array, &to_render, self->index, iteration, self->mixer->cores))
	{
		iteration++;
		i = to_render.x;
		limit_x = to_render.x + (ceil((double) BLOCK_SIZE * self->mixer->cam.aspect_ratio));
		while (i < limit_x && i < RESOLUTION_X)
		{
			ii = to_render.y;
			while (ii < to_render.y + BLOCK_SIZE && ii < RESOLUTION_Y)
			{
				render_ray(self, &(self->mixer->cam.vecs[RESOLUTION_Y - ii - 1][i]), i, ii);
				ii++;
			}
			i++;
		}
	}
}

t_mixer	*copy_mixer(t_mixer *self) {
	t_mixer *ret = ft_gc_malloc(sizeof(t_mixer));
	// TODO make a deep copy
	ret->image = self->image;
	ret->p_mlx_init = self->p_mlx_init;
	ret->p_mlx_window = self->p_mlx_window;
	ret->bounces = 0;
	ret->light_count = self->light_count;
	ret->cam = self->cam;
	ret->col_sum = self->col_sum;
	ret->ambient = self->ambient;
	ret->cores = self->cores;
	return ret;
}

void	rt_forker(t_mixer *mixer)
{
	size_t	i;
	size_t	ii;

	mixer->cores = sysconf(_SC_NPROCESSORS_CONF);
	//mixer->cores = 1;
	mixer->threads = ft_gc_malloc(sizeof(t_thread) * mixer->cores);
	printf("Using %zu threads to render the scene...\n", mixer->cores);
	i = 0;
	ii = 0;
	while (i < mixer->cores)
	{
		mixer->threads[i].mixer = copy_mixer(mixer);//mixer;
		mixer->threads[i].index = i;
		if (pthread_create(&mixer->threads[i].thread, NULL, (t_run) rt_runner, (void *) &mixer->threads[i]) == 0)
			ii++;
		i++;
	}
	if (ii > 0)
	{
		i = 0;
		while (i < mixer->cores)
		{
			pthread_join(mixer->threads[i].thread, NULL);
			i++;
		}
	}
	else
		rt_runner(&mixer->threads[0]);
	printf("Done.\n");
	//exit(0);
}

t_tile	**rt_divide(float aspect)
{
	t_tile	**ret;
	size_t	tiles_per_axis;
	size_t	i;
	size_t	ii;

	tiles_per_axis = ceil((double) RESOLUTION_Y / BLOCK_SIZE);
	//ret = malloc((long) pow(tiles_per_axis, 2) * sizeof(t_tile));
	ret = ft_gc_malloc(tiles_per_axis * sizeof(void *));
	i = 0;
	while (i < tiles_per_axis)
	{
		ii = 0;
		ret[i] = ft_gc_malloc(tiles_per_axis * sizeof(t_tile));
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
