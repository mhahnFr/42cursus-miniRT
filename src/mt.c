/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn   <mhahn@student.42Heilbronn.de      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:01 by mhahn             #+#    #+#             */
/*   Updated: 1970/01/01 00:00:02 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "minirt.h"

void	rt_cleaner(t_mixer *self)
{
	size_t	i;
	size_t	tiles_per_axis;

	tiles_per_axis = ceil((double) self->res_y / BLOCK_SIZE);
	i = 0;
	while (i < tiles_per_axis)
	{
		ft_gc_free(self->tile_array[i]);
		i++;
	}
	ft_gc_free(self->tile_array);
	self->tile_array = NULL;
	i = 0;
	while (i < self->cores)
	{
		free_mixer_copy(self->threads[i].mixer);
		i++;
	}
	ft_gc_free(self->threads);
	self->threads = NULL;
}

void	rt_joiner(size_t i, size_t ii, t_mixer *mixer)
{
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
	printf("\nDone.\n\033[?25h");
	rt_cleaner(mixer);
}

void	rt_forker(t_mixer *mixer)
{
	size_t	i;
	size_t	ii;

	#ifdef WINDOWS
	mixer->cores = 1;
	char   psBuffer[128];
	
	FILE   *pPipe;

	if( (pPipe = _popen( "WMIC CPU Get NumberOfLogicalProcessors", "rt" )) == NULL )
    	exit( 1 );
	int iii = 0;
	while(fgets(psBuffer, 128, pPipe)) {
		iii = 0;
		while (iii < 128 && psBuffer[iii] < '0' && psBuffer[iii] > '9') {
		iii++;
	}
		if (atoi(psBuffer + iii) != 0) {
			mixer->cores = atoi(psBuffer + iii);
			break;
		}
	}
	feof( pPipe);
	#else
	mixer->cores = sysconf(_SC_NPROCESSORS_CONF);
	#endif
	mixer->threads = ft_gc_malloc(sizeof(t_thread) * mixer->cores);
	printf("Using %zu threads to render the scene...\n", mixer->cores);
	printf("\033[?25l       ");
	i = 0;
	ii = 0;
	while (i < mixer->cores)
	{
		mixer->threads[i].mixer = copy_mixer(mixer);
		mixer->threads[i].index = i;
		if (pthread_create(&mixer->threads[i].thread, NULL,
				(void *(*)(void *)) rt_runner, (void *) \
				&mixer->threads[i]) == 0)
			ii++;
		i++;
	}
	rt_joiner(i, ii, mixer);
}

t_tile	**rt_divide(float aspect, t_mixer *mixer)
{
	t_tile	**ret;
	size_t	tiles_per_axis;
	size_t	i;
	size_t	ii;

	tiles_per_axis = ceil((double) mixer->res_y / BLOCK_SIZE);
	ret = ft_gc_malloc(tiles_per_axis * sizeof(void *));
	i = 0;
	while (i < tiles_per_axis)
	{
		ii = 0;
		ret[i] = ft_gc_malloc(tiles_per_axis * sizeof(t_tile));
		while (ii < tiles_per_axis)
		{
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
	mixer->tile_array = rt_divide(mixer->cam.aspect_ratio, mixer);
	rt_forker(mixer);
}
