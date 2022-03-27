/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculator.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:42:53 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/27 21:30:03 by jkasper          ###   ########.fr       */
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
			color = calc_antialiasing(mixer, &(mixer->cam.vecs[i][j]), i, j);
			draw_point(j, i, mixer->image, color);
		}
		printf("%f %f %f \n", mixer->cam.vecs[i][150].x, mixer->cam.vecs[i][150].y, mixer->cam.vecs[i][150].z);
	}
	printf("finished!\n");
}
/*
t_vector	*vector_rand(t_vector *self, float j, float i)
{
	t_vector	inter;
	t_vector	u;
	t_vector	v;
	float		angle_x;
	float		angle_y;

	i += (float) (rand() % 10) / 10;
	j += (float) (rand() % 10) / 10;
	angle_x = (float) j / RESOLUTION_X;
	angle_y = (float) i / RESOLUTION_Y;
	vector_create(&inter, RESOLUTION_X, 0, 0);
	vector_multiply_digit(&u, &inter, angle_x);
	vector_create(&inter, 0, RESOLUTION_Y, 0);
	vector_multiply_digit(&v, &inter, angle_y);
	vector_addition(&inter, &u, &v);
	vector_create(&u, -(((float )RESOLUTION_X) / 2), -(((float) RESOLUTION_Y) / 2), -(((float) RESOLUTION_Y)/ 2));
	vector_addition(self, &u, &inter);
	vector_normalize(self);
	return (self);
}
*/
t_vector	*get_cam_ray(t_vector *self, float j, float i, t_cam cam)
{
	t_vector	inter;
	t_vector	inter2;
	t_vector	inter3;
	float		angle_x;
	float		angle_y;

	//i += (float) (rand() % 10) / 10;
	//j += (float) (rand() % 10) / 10;
	angle_x = (float) j / RESOLUTION_X;
	angle_y = (float) i / RESOLUTION_Y;
	vector_multiply_digit(&inter, &cam.hori, j);
	vector_multiply_digit(&inter2, &cam.vert, i);
	vector_addition(&inter3, &cam.llc, &inter);
	vector_addition(&inter, &inter3, &inter2);
	vector_substract(self, &inter, &cam.position);
	vector_normalize(self);
	
	return (self);
}

t_rgbof	calc_antialiasing(t_mixer *mixer, t_vector *cam_vec, int y, int x)
{
	//antialiasing
	int		i;
	t_rgbof	color;
	t_rgbof	add;

	i = ANTI_ALIASING;
	color = calc_shader(&(mixer->cam.position), cam_vec, mixer);
	color.cal_r = color.r;
	color.cal_g = color.g;
	color.cal_b = color.b;
	while (i > 0)
	{
		//vector_rand has to take t_cam and x,y -> calculate with rand48() a random moved vector inbetween pixels for better result
		add = calc_shader(&(mixer->cam.position), get_cam_ray(cam_vec, x, y, mixer->cam), mixer);
		color = color_add_cal(color, add);
		i--;
	}
	return (color_cal_rgb(color, ANTI_ALIASING + 1));
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
