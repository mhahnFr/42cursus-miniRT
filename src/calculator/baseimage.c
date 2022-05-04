/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baseimage.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 20:23:59 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/20 14:20:34 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "vector.h"

static inline bool	check_side_of_obj(t_mixer *mixer, t_iobj *i_struc)
{
	float		t;
	float		t2;
	t_vector	inter;
	t_vector	inter2;
	t_obj_l		*obj;

	obj = i_struc->obj_col;
	if (obj->obj_type != PLANE && obj->obj_type != CYLINDER_CAP)
		return (true);
	vector_substract(&inter, &obj->position, &mixer->cam.position);
	vector_substract(&inter2, &obj->position, &i_struc->light->position);
	t = vector_scalar_product(&inter, &obj->normal);
	t2 = vector_scalar_product(&inter2, &obj->normal);
	return ((t * t2) > 0.0f);
}

static inline void	trace_light_loop(t_mixer *mixer,
										t_vector *vecs[2], t_iobj *i_struc)
{
	if (i_struc->light->obj_type == LIGHT)
	{
		vector_substract(&i_struc->ray, &i_struc->light->position, \
					vecs[0]);
		i_struc->length = vector_length(&i_struc->ray);
		vector_normalize(&i_struc->ray);
		i_struc->origin = *vecs[0];
		if (check_side_of_obj(mixer, i_struc))
		{
			intersect_object(mixer, i_struc, i_struc->length);
			if (!i_struc->shadow)
				vector_addition(&i_struc->stack_vecs[2], \
			&i_struc->stack_vecs[2], &i_struc->ret_color);
		}
	}
	i_struc->light = i_struc->light->next;
}

t_vector	trace_light(t_mixer *mixer, t_obj_l *curr, t_vector *vecs[2])
{
	t_iobj		i_struc;

	i_struc.obj_col = curr;
	i_struc.stack_vecs[2] = rgbof_cast_vector(curr->color);
	i_struc.stack_vecs[3] = rgbof_cast_vector(mixer->ambient.color);
	vector_multiply_digit(&i_struc.stack_vecs[2], &i_struc.stack_vecs[2], \
	mixer->ambient.a_light);
	vector_multiply(&i_struc.stack_vecs[2], &i_struc.stack_vecs[2], \
					&i_struc.stack_vecs[3]);
	i_struc.light = mixer->obj_list;
	while (i_struc.light != NULL)
	{
		trace_light_loop(mixer, vecs, &i_struc);
	}
	return (i_struc.stack_vecs[2]);
}

t_vector	trace_rand(t_vector ray, t_vector normal, float diffusion)
{
	t_vector	tmp;
	t_vector	reflection;
	float		inter;
	float		rand_val;

	inter = vector_scalar_product(&ray, &normal);
	if (inter < 0)
		inter *= -1;
	vector_multiply_digit(&reflection, &normal, inter * 2);
	vector_addition(&reflection, &reflection, &ray);
	if (diffusion == 0)
		return (reflection);
	rand_val = (float) drand48();
	tmp.x = ((rand_val - 0.5f) * diffusion) + reflection.x;
	rand_val = (float) drand48();
	tmp.y = ((rand_val - 0.5f) * diffusion) + reflection.y;
	rand_val = (float) drand48();
	tmp.z = ((rand_val - 0.5f) * diffusion) + reflection.z;
	vector_normalize(&tmp);
	vector_addition(&tmp, &tmp, &reflection);
	return (tmp);
}
