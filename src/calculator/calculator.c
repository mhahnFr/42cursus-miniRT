#include "minirt.h"

void	calculator(t_mixer *mixer, int *ret)
{
	calc_object_ray(mixer, ret);
}

void	calc_object_ray(t_mixer *mixer, int *ret)
{
	for (int i = 0; i < RESOLUTION_Y; i++) {
		for (int j = 0; j < RESOLUTION_X; j++) {
			calc_intersec_objs(mixer->obj_list, &mixer->cam.normal);
		}
	}
}

void	calc_intersec_objs(t_obj_l *objs, t_vector *vec)
{
	t_vector	intersect;

	while (objs != NULL)
	{
		if (objs->obj_type == SPHERE)
		{
			// TODO
		}
		else if (objs->obj_type == PLANE && calc_intersecs_plane(vec, &objs->normal))
			intersect = calc_intersection_plane();
		else if (objs->obj_type == CYLINDER)
		{
			// TODO
		}
		objs = objs->next;
	}
}

t_vector	calc_intersection_plane()
{
	t_vector	point;

	;
	return (point);
}

bool	calc_intersecs_plane(t_vector *vec, t_vector *normal)
{
	return (!vector_scalar_product(vec, normal));
}
