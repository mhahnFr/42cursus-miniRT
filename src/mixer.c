/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mixer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:51:25 by mhahn             #+#    #+#             */
/*   Updated: 2022/03/11 16:39:09 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "mlx.h"
#include "renderer_image.h"
#include "minirt.h"

t_mixer	*init_mainstruct(int *err)
{
	t_mixer	*ret;

	ret = malloc(sizeof(struct s_mixer));
	if (ret == NULL)
	{
		*err = 3;
		return (NULL);
	}
	ret->p_mlx_init = mlx_init();
	ret->image = renderer_image_new(ret->p_mlx_init, RESOLUTION_X, RESOLUTION_Y);
	//ret->image = mlx_new_image(ret->p_mlx_init, RESOLUTION_X, RESOLUTION_Y);
	return (ret);
}
