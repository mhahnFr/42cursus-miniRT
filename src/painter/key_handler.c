/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 18:15:32 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/05 13:15:57 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "minirt.h"

void	key_escape(t_mixer *p_null)
{
	mlx_destroy_window(p_null->p_mlx_init, p_null->p_mlx_window);
	renderer_image_delete(p_null->image);
	ft_gc_clear();
	exit(0);
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
