/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn   <mhahn@student.42Heilbronn.de      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:01 by mhahn             #+#    #+#             */
/*   Updated: 1970/01/01 00:00:02 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "minirt.h"

static inline void	render_ray(t_thread *self,
								t_vector *ray,
								size_t x,
								size_t y)
{
	t_rgbof	color;

	color = calc_first_ray_of_the_day(self->mixer, ray);
	draw_point(x, y, self->mixer->image, color);
}

/*
 * Old block fetcher:
   i = 0;
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
 */

static bool	rt_block_fetcher(t_tile **tile_array,
							t_tile *render,
							size_t iteration,
							t_thread *self)
{
	size_t	tiles_per_axis;
	size_t	next;
	size_t	x;
	size_t	y;

	tiles_per_axis = ceil((double) RESOLUTION_Y / BLOCK_SIZE);
	next = self->index + (self->mixer->cores * iteration);
	x = next % tiles_per_axis;
	y = next / tiles_per_axis;
	if (x < tiles_per_axis && y < tiles_per_axis)
	{
		*render = tile_array[x][next / tiles_per_axis];
		return (true);
	}
	return (false);
}

void	rt_runner(t_thread *self)
{
	t_tile	to_render;
	size_t	numbers[4];

	numbers[3] = 0;
	while (rt_block_fetcher(self->mixer->tile_array, &to_render,
			numbers[3], self))
	{
		numbers[3]++;
		numbers[0] = to_render.x;
		numbers[2] = to_render.x + (ceil((double) BLOCK_SIZE
					* self->mixer->cam.aspect_ratio));
		while (numbers[0] < numbers[2] && numbers[0] < RESOLUTION_X)
		{
			numbers[1] = to_render.y;
			while (numbers[1] < to_render.y + BLOCK_SIZE
				&& numbers[1] < RESOLUTION_Y)
			{
				render_ray(self,
					&(self->mixer->cam.vecs[RESOLUTION_Y - numbers[1] - 1] \
					[numbers[0]]), numbers[0], numbers[1]);
				numbers[1]++;
			}
			numbers[0]++;
		}
	}
}
