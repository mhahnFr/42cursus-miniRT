/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 18:15:32 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/13 14:05:09 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "minirt.h"

void	key_escape(t_mixer *p_null)
{
	t_obj_l	*tmp;
	t_obj_l	*tmp2;
	int		i;

	mlx_destroy_window(p_null->p_mlx_init, p_null->p_mlx_window);
	renderer_image_delete(p_null->image);
	tmp = p_null->obj_list;
	while (tmp != NULL)
	{
		tmp2 = tmp->next;
		ft_gc_free(tmp2);
		tmp = tmp2;
	}
	ft_gc_free(p_null->col_sum.sum);
	ft_gc_free(p_null->col_sum.fac);
	ft_gc_free(p_null);
	ft_gc_exit(0);
}

int	key_handler(int key, t_mixer *p_null)
{
	if (key == ESC_KEY)
		key_escape(p_null);
	return (0);
}

int	key_redcross(t_mixer *p_null)
{
	key_escape(p_null);
	return (0);
}
