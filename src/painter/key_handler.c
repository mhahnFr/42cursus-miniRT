/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 18:15:32 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/18 22:33:44 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "minirt.h"

void	key_escape(t_mixer *p_null)
{
	ft_gc_clear();
	(void) p_null;
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
	return (1);
}
