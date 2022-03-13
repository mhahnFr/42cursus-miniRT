/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculator.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 21:42:53 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/13 15:38:52 by jkasper          ###   ########.fr       */
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
	(void) ret;
	for (int i = 0; i < RESOLUTION_Y; i++) {
		for (int j = 0; j < RESOLUTION_X; j++) {
			//printf("x: %i y: %i\n", j, i);
			calc_intersec_first(mixer, &(mixer->cam.vecs[i][j]), i, j);
		}
	}
	printf("finished!\n");
}

void	skip_obj(t_obj_l **objs, int toskip)
{
	while ((*objs) != NULL && (*objs)->obj_type == toskip)
		(*objs) = (*objs)->next;
}


bool	calc_intersection_sphere(t_cam cam, t_obj_l *obj, t_vector *ray, t_vector *ret)
{
	float		a;
	float		b;
	float		c;
	float		d;
	t_vector	OC;
	
	vector_substract(&OC, &(cam.position), &(obj->position));
	a = vector_scalar_product(ray, ray);
	b = vector_scalar_product(&OC, ray);
	c = vector_scalar_product(&OC, &OC) - powf(obj->height, 2);
	d = powf(b,2) - a *c;
	if (d < 0)
		return (false);
	float	temp = (-b - sqrtf(d) ) / a;
	if (temp  > 0)
	{
		obj->disthit = temp;
		vector_multiply_digit(&OC, ray, temp);
		vector_addition(ret, &(cam.position), &OC);
		vector_substract(&OC, ret, &(obj->position));
		return (true);
	}
	temp = (-b + sqrtf(d)) / a;
	if (temp > 0)
	{
		obj->disthit = temp;
		vector_multiply_digit(&OC, ray, temp);
		vector_addition(ret, &(cam.position), &OC);
		vector_substract(&OC, ret, &(obj->position));
		vector_division(&(obj->col_normal), &OC, obj->height);
		return (true);
	}
	return (false);
}

bool	calc_intersec_next(t_obj_l *objs, t_mixer *mixer, t_vector *ray, t_vector	*inter)
{
	if (objs->obj_type == SPHERE)
		return (calc_intersection_sphere(mixer->cam, objs, ray, inter));
	else if (objs->obj_type == PLANE && calc_intersecs_plane(ray, &objs->normal))
		return (calc_intersection_plane(ray, objs, inter));
	//else if (objs->obj_type == CYLINDER)
	//	return (calc_intersection_cylinder(mixer->cam, objs, ray))
	return (false);
}

bool	calc_intersec_dist(t_vector intersect, t_vector new_intersect, t_vector *cam)
{
	t_vector	inter;
	t_vector	inter2;

	vector_substract(&inter, &intersect, cam);
	vector_substract(&inter2, &new_intersect, cam);
	return (vector_length(&inter) < vector_length(&inter2));
}

t_rgbof	calc_intersec_first(t_mixer *mixer, t_vector *ray, int y, int x)
{
	t_vector	*intersect;
	t_vector	*new_intersect;
	t_obj_l		*objs;
	t_rgbof		black;
	float		distsf;
	bool		sw;
	bool		first;
	bool		secon;

	objs = mixer->obj_list;
	sw = false;
	intersect = ft_calloc(1, sizeof(t_vector));
	new_intersect = ft_calloc(1, sizeof(t_vector));
	first = calc_intersec_next(objs, mixer, ray, intersect);
	objs = objs->next;
	black.r = (255 - (y % 255)) / 2;
	black.g = 255 - (y % 255);
	black.b = 255;
	while (objs != NULL)
	{
		if (sw == false && calc_intersec_next(objs, mixer, ray, intersect))
		{
			distsf = objs->disthit;
			black = objs->color;
			sw = true;
		}
		else if (sw == true)
		{
			if (calc_intersec_next(objs, mixer, ray, new_intersect) && distsf > objs->disthit)
			{
				intersect = new_intersect;
				black = objs->color;	
			}
		}
		objs = objs->next;
	}
	draw_point(x, y, mixer->image, black);
	free(intersect);
	free(new_intersect);
	return (black);
}

bool	calc_intersection_plane(t_vector *cam, t_obj_l *objs, t_vector *point)
{
	t_vector	vec2;
	t_vector	vec_inter;
	float		d;

	vec2.x = cam->x;
	vec2.y = cam->y;
	vec2.z = cam->z;
	vector_normalize(&vec2);
	vector_multiply_digit(&vec_inter, &vec2, 0.5);
	vector_multiply(&vec2, &vec_inter, cam);
	vector_substract(&vec_inter, &(objs->position), &vec2);
	d = vector_scalar_product(&vec_inter, &(objs->normal)) / vector_scalar_product(cam, &(objs->normal));
	if (d < 0)
		return (false);
	vector_multiply_digit(&vec_inter, cam, d);
	vector_addition(point, &vec2, &vec_inter);
	objs->disthit = d;
	return (true);
}

bool	calc_intersecs_plane(t_vector *vec, t_vector *normal)
{
	return (vector_scalar_product(vec, normal) != 0);
}
