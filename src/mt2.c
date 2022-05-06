/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   <error>                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn   <mhahn@student.42Heilbronn.de      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:01 by mhahn             #+#    #+#             */
/*   Updated: 1970/01/01 00:00:02 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_mixer_copy(t_mixer *self)
{
	t_obj_l	*tmp;
	t_obj_l	*tmp2;

	tmp = self->obj_list;
	while (tmp != NULL)
	{
		tmp2 = tmp->next;
		ft_gc_free(tmp);
		tmp = tmp2;
	}
	ft_gc_free(self->col_sum.sum);
	ft_gc_free(self->col_sum.fac);
	ft_gc_free(self);
}

static void	append_obj(t_obj_l **head, t_obj_l *appendix)
{
	t_obj_l	*tmp;

	tmp = *head;
	if (tmp == NULL)
	{
		*head = appendix;
		appendix->prev = NULL;
		appendix->next = NULL;
		return ;
	}
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = appendix;
	appendix->prev = tmp;
	appendix->next = NULL;
}

static inline t_obj_l	*copy_obj(t_obj_l *self)
{
	t_obj_l	*ret;

	ret = ft_gc_malloc(sizeof(t_obj_l));
	ret->emitter = self->emitter;
	ret->inv_normal = self->inv_normal;
	ret->height = self->height;
	ret->width = self->width;
	ret->depth = self->depth;
	ret->brightness = self->brightness;
	ret->diffusion = self->diffusion;
	ret->disthit = self->disthit;
	ret->intensity = self->intensity;
	ret->reflec_fac = self->reflec_fac;
	ret->obj_type = self->obj_type;
	ret->cube_side = self->cube_side;
	ret->color = self->color;
	ret->col_normal = self->col_normal;
	ret->normal = self->normal;
	ret->normal_down = self->normal_down;
	ret->normal_left = self->normal_left;
	ret->position = self->position;
	return (ret);
}

static inline t_obj_l	*copy_objs(t_mixer *self)
{
	t_obj_l	*ret;
	t_obj_l	*old;
	t_obj_l	*curr;

	ret = NULL;
	old = self->obj_list;
	while (old != NULL)
	{
		curr = copy_obj(old);
		append_obj(&ret, curr);
		old = old->next;
	}
	return (ret);
}

t_mixer	*copy_mixer(t_mixer *self)
{
	t_mixer	*ret;

	ret = ft_gc_malloc(sizeof(t_mixer));
	ret->image = self->image;
	ret->p_mlx_init = self->p_mlx_init;
	ret->p_mlx_window = self->p_mlx_window;
	ret->bounces = 0;
	ret->light_count = self->light_count;
	ret->ambient = self->ambient;
	ret->cam = self->cam;
	ret->col_sum = self->col_sum;
	ret->col_sum.sum
		= ft_calloc(1, (self->light_count + 2) * sizeof(t_vector));
	ret->col_sum.fac = ft_calloc(1, (self->light_count + 2) * sizeof(float));
	ret->diff_sh = self->diff_sh;
	ret->obj_list = copy_objs(self);
	ret->tile_array = self->tile_array;
	ret->cores = self->cores;
	ret->res_x = self->res_x;
	ret->res_y = self->res_y;
	ret->max_bounces = self->max_bounces;
	ret->antialiasing = self->antialiasing;
	return (ret);
}
