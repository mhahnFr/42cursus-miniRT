/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diffuse_intersection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 17:17:00 by mhahn             #+#    #+#             */
/*   Updated: 2022/04/26 17:17:02 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline bool	diffuse_next(
		t_obj_l *obj,
		t_vector *start,
		t_vector *ray,
		t_vector *inter)
{
	bool	ret;

	ret = false;
	if (obj->obj_type == SPHERE)
		ret = hit_sphere(start, obj, ray, inter);
	else if (obj->obj_type == PLANE && fast_intersec_plane(ray, &obj->normal))
		ret = intersec_plane(ray, start, obj, inter);
	else if (obj->obj_type >= CYLINDER_CAP)
		return (hit_cylinder(start, obj, ray, inter));
	return (ret);
}

bool	diffuse_nearest(
		t_mixer *mixer,
		t_diff *diff,
		t_vector *start,
		t_vector *result)
{
	t_vector	vecs[2];
	t_obj_l		*objs;
	t_obj_l		*curr;
	float		distsf;
	bool		sw;

	objs = mixer->obj_list;
	sw = false;
	while (objs != NULL)
	{
		if (diff->hit != objs && diffuse_next(objs, start, diff->ray, \
		&vecs[0]) && (!sw || distsf > objs->disthit))
		{
			distsf = objs->disthit;
			vecs[1] = vecs[0];
			sw = true;
			curr = objs;
		}
		objs = objs->next;
	}
	*result = vecs[1];
	diff->hit = curr;
	return (sw);
}
