/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diffuse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 19:37:04 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/20 16:03:30 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline t_vector	diffuse_rand(t_diff diff)
{
	t_vector	tmp;
	t_vector	reflection;
	float		x;
	float		y;
	float		z;

	x = vector_scalar_product(diff.ray, &diff.hit->col_normal);
	if (x < 0)
		x *= -1;
	vector_multiply_digit(&reflection, &diff.hit->col_normal, x * 2);
	vector_addition(&reflection, &reflection, diff.ray);
	x = (float) drand48() + reflection.x;
	y = (float) drand48() + reflection.y;
	z = (float) drand48() + reflection.z;
	tmp.x = (x - 0.5f) * (diff.hit->diffusion * 0.5f);
	tmp.y = (y - 0.5f) * (diff.hit->diffusion * 0.5f);
	tmp.z = (z - 0.5f) * (diff.hit->diffusion * 0.5f);
	vector_normalize(&tmp);
	vector_addition(&tmp, &tmp, &reflection);
	return (tmp);
}

//t_vector	diffuse_randi1() {
//	t_vector ret;
//	t_vector inter;
//
//	do {
//		vector_create(&inter, drand48(), drand48(), drand48());
//		vector_multiply_digit(&inter, &inter, 2);
//		vector_create(&ret, inter.x - 1, inter.y - 1, inter.z - 1);
//	} while ((ret.x * ret.x + ret.y * ret.y + ret.z * ret.z) >= 1);
//	return (ret);
//}

t_rgbof	vector_cast_rgbof(t_vector vec)
{
	t_rgbof	c;

	if (vec.x > 1)
		vec.x = 1;
	c.r = vec.x * 255;
	if (vec.y > 1)
		vec.y = 1;
	c.g = vec.y * 255;
	if (vec.z > 1)
		vec.z = 1;
	c.b = vec.z * 255;
	return (c);
}

t_vector	rgbof_cast_vector(t_rgbof color)
{
	t_vector	vec;

	vec.x = (float) color.r / 255;
	vec.y = (float) color.g / 255;
	vec.z = (float) color.b / 255;
	return (vec);
}

t_vector	diffuse_get(t_mixer *mixer, t_diff diff, t_vector *result)
{
	t_vector	inter;
	t_vector	inter2;
	t_vector	inter3;
	t_vector	s;
	t_vector	ret;

	if (diff.ray_count < mixer->max_bounces
		&& diffuse_nearest(mixer, &diff, diff.origin, result))
	{
		*diff.ray = diffuse_rand(diff);
		vector_create(&s, result->x, result->y, result->z);
		diff.origin = &s;
		diff.ray_count += 1;
		inter2 = diffuse_get(mixer, diff, result);
		inter3 = rgbof_cast_vector(diff.hit->color);
		vector_multiply(&inter2, &inter2, &inter3);
		vector_multiply_digit(&inter, &inter2, mixer->ambient.a_light);
		return (inter);
	}
	else
	{
		ret = rgbof_cast_vector(mixer->ambient.color);
		return (ret);
	}
}

t_vector	diffuse_main(t_mixer *mixer, t_obj_l *obj, t_vector *intersect)
{
	t_vector	ret;
	t_vector	s;
	t_vector	res;

	mixer->diff_sh.ray = intersect;
	mixer->diff_sh.hit = obj;
	mixer->diff_sh.ray_count = 0;
	vector_create(&s, mixer->cam.position.x,
		mixer->cam.position.y, mixer->cam.position.z);
	mixer->diff_sh.origin = &s;
	vector_create(&res, 0, 0, 0);
	ret = diffuse_get(mixer, mixer->diff_sh, &res);
	if (mixer->diff_sh.ray_count < 1)
		mixer->diff_sh.ray_count = 1;
	return (ret);
}
