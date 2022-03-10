/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:49:44 by mhahn             #+#    #+#             */
/*   Updated: 2022/03/09 22:02:59 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "minirt.h"
#include "matrix.h"

void	rt_cam_init(t_mixer *mixer)
{
	t_matrix	*matrix;
	size_t		i;
	size_t		j;
	float		angle_x;
	float		angle_y;

	i = 0;
	matrix = matrix_new(NULL, 3, 3);
	angle_x = -(mixer->cam.fov / 2);
	angle_y = (mixer->cam.fov / mixer->cam.aspect_ratio) / 2;
	while (i < RESOLUTION_Y)
	{
		j = 0;
		angle_y -= (mixer->cam.fov / mixer->cam.aspect_ratio) / RESOLUTION_Y;
		while (j < RESOLUTION_X)
		{
			angle_x += mixer->cam.fov / RESOLUTION_X;
			matrix->values[0][0] = cos(angle_x);
			matrix->values[0][1] = -sin(angle_x);
			matrix->values[0][2] = 0;

			matrix->values[1][0] = sin(angle_y);
			matrix->values[1][1] = cos(angle_y);
			matrix->values[1][2] = 0;

			matrix->values[2][0] = 0;
			matrix->values[2][1] = 0;
			matrix->values[2][2] = 1;
			matrix_multiply_vector(&mixer->cam.vecs[i][j], matrix, &mixer->cam.normal);
			j++;
		}
		i++;
	}
}
