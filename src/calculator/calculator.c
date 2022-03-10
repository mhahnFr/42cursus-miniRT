#include <stdio.h>
#include "minirt.h"

void	calculator(t_mixer *mixer, int *ret)
{
	calc_object_ray(mixer, ret);
}

void	calc_object_ray(t_mixer *mixer, int *ret)
{
	(void) ret;
	for (int i = 0; i < RESOLUTION_Y; i++) {
		for (int j = 0; j < RESOLUTION_X; j++) {
			calc_intersec_objs(mixer->obj_list, &(mixer->cam.vecs[i][j]));
		}
	}
}

void	calc_intersec_objs(t_obj_l *objs, t_vector *cam)
{
	t_vector	intersect;

	while (objs != NULL)
	{
		if (objs->obj_type == SPHERE)
		{
			// TODO
		}
		else if (objs->obj_type == PLANE && calc_intersecs_plane(cam, &objs->normal))
		{
			intersect = calc_intersection_plane(cam, objs);
			printf("x:%f y:%f z:%f\n", intersect.x, intersect.y, intersect.z);
		}
		else if (objs->obj_type == CYLINDER)
		{
			// TODO
		}
		objs = objs->next;
	}
}

t_vector	calc_intersection_plane(t_vector *cam, t_obj_l *objs)
{
	t_vector	vec2;
	t_vector	point;
	t_vector	vec_inter;
	float		d;

	vector_create(&vec2, 0, 0, 0);
	vector_create(&vec_inter, 0, 0, 0);
	vector_create(&point, 0, 0, 0);
	vec2.x = cam->x;
	vec2.y = cam->y;
	vec2.z = cam->z;
	vector_normalize(&vec2);
	printf("x:%f y:%f z:%f\n", vec2.x, vec2.y, vec2.z);
	printf("PLANE\n");
	vector_multiply_digit(&vec_inter, &vec2, 0.5);
	printf("x:%f y:%f z:%f\n", vec_inter.x, vec_inter.y, vec_inter.z);
	printf("x:%f y:%f z:%f\n", cam->x, cam->y, cam->z);
	vector_multiply(&vec2, &vec_inter, cam);
	printf("x:%f y:%f z:%f\n", vec2.x, vec2.y, vec2.z);
	vector_substract(&vec_inter, &(objs->position), &vec2);
	printf("x:%f y:%f z:%f\n", vec_inter.x, vec_inter.y, vec_inter.z);
	printf("a:%f b:%f \n",vector_scalar_product(&vec_inter, &(objs->normal)), vector_scalar_product(cam, &(objs->normal)));
	d = vector_scalar_product(&vec_inter, &(objs->normal)) / vector_scalar_product(cam, &(objs->normal));
	printf("d:%f\n", d);
	vector_multiply_digit(&vec_inter, cam, d);
	printf("x:%f y:%f z:%f\n", vec_inter.x, vec_inter.y, vec_inter.z);
	vector_addition(&point, &vec2, &vec_inter);
	return (point);
}

bool	calc_intersecs_plane(t_vector *vec, t_vector *normal)
{
	return (vector_scalar_product(vec, normal) != 0);
}
