/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 18:15:32 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/13 15:00:49 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "MLX42/MLX42.h"
#include "minirt.h"

static inline void	free_cam_vecs(t_mixer *self)
{
	int32_t	i;

	i = 0;
	while (i < self->res_y)
	{
		ft_gc_free(self->cam.vecs[i]);
		i++;
	}
	ft_gc_free(self->cam.vecs);
}

void	key_escape(t_mixer *p_null)
{
	t_obj_l	*tmp;
	t_obj_l	*tmp2;

	renderer_image_delete(p_null->image);
	mlx_terminate(p_null->mlx);
	tmp = p_null->obj_list;
	while (tmp != NULL)
	{
		tmp2 = tmp->next;
		ft_gc_free(tmp);
		tmp = tmp2;
	}
	ft_gc_free(p_null->col_sum.sum);
	ft_gc_free(p_null->col_sum.fac);
	free_cam_vecs(p_null);
	ft_gc_free(p_null);
	ft_gc_exit(0);
}

void key_handler(int key, t_mixer *p_null)
{
	if (key == MLX_KEY_ESCAPE)
		key_escape(p_null);
}

void	key_redcross(t_mixer *p_null)
{
	key_escape(p_null);
}
