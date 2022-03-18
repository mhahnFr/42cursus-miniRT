/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculator.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:42:53 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/18 16:32:16 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minirt.h"
#include "renderer_image.h"
#include <math.h>
#include <stdbool.h>

void	draw_point(int x, int y, t_renderer_image *buf, t_rgbof color)
{
	char	*dst;

	dst = buf->raw + (long)x * (buf->depth / 8) + (long)y
		* buf->line_size;
	*(unsigned int *) dst = (0 << 24) + (color.r << 16) + (color.g << 8)
	+ (color.b << 0);
}

void	calculator(t_mixer *mixer, int *ret)
{
	calc_object_ray(mixer, ret);
}

void	calc_object_ray(t_mixer *mixer, int *ret)
{
	t_rgbof	color;
	(void) ret;
	for (int i = 0; i < RESOLUTION_Y; i++) {
		for (int j = 0; j < RESOLUTION_X; j++) {
			color = calc_cam_ray(mixer, &(mixer->cam.vecs[i][j]), i, j);
			draw_point(j, i, mixer->image, color);
		}
	}
	printf("finished!\n");
}

t_rgbof	calc_cam_ray(t_mixer *mixer, t_vector *cam_vec, int y, int x)
{
	//antialiasing
	int		i;
	t_rgbof	color;
	t_rgbof	add;

	i = ANTI_ALIASING;
	color = calc_intersect_vector(NULL, mixer->obj_list, &(mixer->cam.position), cam_vec);
	color.cal_r = color.r;
	color.cal_r = color.g;
	color.cal_r = color.b;
	while (i > 0)
	{
		add = calc_intersect_vector(NULL, mixer->obj_list, &(mixer->cam.position), cam_vec);
		color_add_cal(color, add);
		i--;
	}
	return (color_cal_rgb(color, ANTI_ALIASING + 1));
//	return (color);
}

//LEGACY:
/*


void	skip_obj(t_obj_l **objs, int toskip)
{
	while ((*objs) != NULL && (*objs)->obj_type == toskip)
		(*objs) = (*objs)->next;
}

bool	calc_intersec_next(t_obj_l *objs, t_vector *origin, t_vector *ray, t_vector	*inter)
{
	bool	ret;

	ret = false;
	if (objs->obj_type == SPHERE)
		ret = hit_sphere(origin, objs, ray, inter);
	else if (objs->obj_type == PLANE && calc_intersecs_plane(ray, &objs->normal))
	{
		ret = calc_intersection_plane(ray, objs, inter);
		objs->disthit = vector_distance(origin, inter);
	}
	//else if (objs->obj_type == CYLINDER)
	//	return (calc_intersection_cylinder(mixer->cam, objs, ray))
	return (ret);
}

bool	calc_intersec_dist(t_vector intersect, t_vector new_intersect, t_vector *cam)
{
	t_vector	inter;
	t_vector	inter2;

	vector_substract(&inter, &intersect, cam);
	vector_substract(&inter2, &new_intersect, cam);
	return (vector_length(&inter) < vector_length(&inter2));
}


t_rgbof	calc_random_rays(t_mixer *mixer, t_vector *ray, int y, int x)
{
	t_rgbof	res_col;

	res_col.r = (255 - (y / 255)) / 2;
	res_col.g = 255 - (y / 255);
	res_col.b = 254;
	res_col	= calc_intersec_first(mixer, ray, res_col);
	return (res_col);
}

t_rgbof	calc_intersec_first(t_mixer *mixer, t_vector *ray, t_rgbof pcolor)
{
	t_vector	*intersect;
	t_vector	*new_intersect;
	t_obj_l		*objs;
	t_obj_l		*curr;
	float		distsf;
	bool		sw;

	objs = mixer->obj_list;
	curr = NULL;
	sw = false;
	intersect = ft_calloc(1, sizeof(t_vector));
	new_intersect = ft_calloc(1, sizeof(t_vector));
	while (objs != NULL)
	{
		if (sw == false && calc_intersec_next(objs, &(mixer->cam.position), ray, intersect))
		{
			distsf = objs->disthit;
			curr = objs;
			sw = true;
		}
		else if (sw == true)
		{
			if (calc_intersec_next(objs, &(mixer->cam.position), ray, new_intersect) && distsf > objs->disthit)
			{
				intersect = new_intersect;
				distsf = objs->disthit;
				curr = objs;
			}
			else
				free(new_intersect);
		}
		objs = objs->next;
	}
	if (curr != NULL)
	pcolor = curr->color;
	if (curr != NULL)
		pcolor = diffuse_get(mixer, curr, intersect);
	return (pcolor);
}
*/
