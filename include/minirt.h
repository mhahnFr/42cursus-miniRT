/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:50:22 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/11 14:14:10 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdbool.h>
# include "vector.h"
# include "renderer_image.h"
# include "libft.h"

/* Resolution in Pixel */
# define RESOLUTION_X	1000
# define RESOLUTION_Y	1000
# define ANTI_ALIASING	10
# define MAX_BOUNCES	1
/* Object-types */
# define CAMERA		   -2
# define AMBIENT	   -1
# define LIGHT			0
# define SPHERE			1
# define PLANE			2
# define CYLINDER		3
# define START			4

# define LEXER_BUFFER	100
# define ESC_KEY		53

/*
 * Red green blue values for an object or light 
 * Inits with white: 255, 255, 255
 * RED GREEN BLUE OPACITY REFLECTIVITY
 */
typedef struct s_rgbof {
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	o;
	unsigned char	f;

	unsigned int	cal_r;
	unsigned int	cal_g;
	unsigned int	cal_b;
}	t_rgbof;

/*
 * Object list for all lights, and objects
 *
 * WIDTH: in case of a sphere width and height are both the same
 */
typedef struct s_object_l {
	bool				emitter;
	bool				inv_normal;
	float				height;
	float				width;
	float				brightness;
	float				diffusion;
	float				disthit;
	float				intensity;
	float				reflec_fac;
	float				max_length;
	int					obj_type;
	t_rgbof				color;
	t_vector			col_normal;
	t_vector			normal;
	t_vector			position;

	struct s_object_l	*next;
	struct s_object_l	*prev;
}	t_obj_l;

typedef struct s_ambient {
	float	a_light;
	t_rgbof	color;
}	t_ambient;

typedef struct s_diffuse
{
	int			ray_count;
	t_obj_l		*hit;
	t_vector	*origin;
	t_vector	*ray;
}	t_diff;

/*
 * Camera struct 
 * FOV between 0-180
 */
typedef struct s_cam {
	float		aspect_ratio;
	int			fov;
	t_vector	hori;
	t_vector	llc;
	t_vector	normal;
	t_vector	position;
	t_vector	step;
	t_vector	vecs[RESOLUTION_Y][RESOLUTION_X];
	t_vector	vert;
}	t_cam;

/* Color_calcstruct */
typedef struct s_col_calc
{
	bool		sw;
	float		*fac;
	int			l_count;
	t_vector	*sum;
	t_vector	basecolor;
	t_vector	diff;
}	t_col;

/* Mainstruct for MiniRT */
typedef struct s_mixer {
	t_renderer_image	*image;
	void				*p_mlx_init;
	void				*p_mlx_window;

	int					bounces;
	int					light_count;
	struct s_ambient	ambient;
	struct s_cam		cam;
	struct s_col_calc	col_sum;
	t_diff				diff_sh;
	t_obj_l				*obj_list;
}	t_mixer;

//needed for calculator, for norm reason
typedef struct s_iobj {
	t_vector	ray;
	t_vector	origin;
	t_vector	inter;
	t_vector	inter_final;
	t_vector	ret_color;
	t_obj_l		*list;
	t_obj_l		*curr;
	t_obj_l		*light;
	t_obj_l		*obj_col;
}	t_iobj;

/* Functions */

/*				Main */
/*
 * Creates the main delegate object. Takes a pointer to an integer in which to
 * store the error code.
 */
t_mixer		*init_mainstruct(int *err);

/*
 * Initializes the values of the camera. Takes the mixer object as parameter.
 */
void		rt_cam(t_mixer *mixer);

/*				Painter */
/*
 * Paints the whole scene. Takes the delegate with all objects and
 * precalculated values as well as a pointer to an integer, in which the error
 * code is saved.
 */
void		paint(t_mixer *delegate, int *ret);

/*				Calculator */
void		calculator(t_mixer *mixer, int *ret);

/*
 * Iterates over all vectors of the camera stored in the given mixer object and
 * calculates their intersecting vectors, if there are any.
 */
void		calc_object_ray(t_mixer *mixer, int *ret);

/*
 * Skips the objects of the given type at the beginning of the given list.
 */
void		skip_obj(t_obj_l **objs, int toskip);

/*
 * Calculates the intersecting vector of the given object, which is treated as
 * a sphere. Takes the camera object, the sphere object and the ray for which
 * to calculate the intersecting vector.
 */
bool		hit_sphere(
				t_vector *origin,
				t_obj_l *obj,
				t_vector *ray,
				t_vector *ret);

//calculates the color of a pixel if obj and light present
bool		intersect_object(
				t_mixer *mixer,
				t_iobj *i_struc,
				float length);

/*
 * Calculates the intersecting vector for the given ray vector for the next
 * object in the given list.
 */
bool		calc_intersec_next(
				t_obj_l *objs,
				t_vector *mixer,
				t_vector *ray,
				t_vector *ret);

/*
 * Returns wether the first intersecting vector is closer to the given camera
 * vector or the second one.
 */
bool		calc_intersec_dist(
				t_vector intersect,
				t_vector new_intersect,
				t_vector *cam);

t_rgbof		calc_random_rays(t_mixer *mixer, t_vector *ray, int y, int x);

/*
 * Calculates the first intersecting vector for the given ray vector and the
 * given object list.
 */
t_rgbof		calc_intersec_first(t_mixer *mixer, t_vector *ray, t_rgbof pcolor);

bool		diffuse_nearest(
				t_mixer *mixer,
				t_diff *diff,
				t_vector *start,
				t_vector *result);

/*
 * Calculates the intersecting vector for the given object and the camera
 * vector. The given object is expected to be a plane.
 */
bool		intersec_plane(
				t_vector *cam,
				t_vector *cam_pos,
				t_obj_l *objs,
				t_vector *ret);

/*
 * Returns wether the given vector intersects an infinite plane with the given
 * normal.
 */
bool		fast_intersec_plane(t_vector *vec, t_vector *normal);

bool		hit_cylinder(
				t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *inter);

t_rgbof		calc_shader(
				t_vector *origin,
				t_vector *ray,
				t_mixer *mixer,
				t_col *colsum);

t_rgbof		calc_antialiasing(
				t_mixer *mixer,
				t_vector *cam_vec,
				t_rgbof color);

t_rgbof		calc_first_ray_of_the_day(t_mixer *mixer, t_vector *cam_vec);

t_vector	diffuse_main(t_mixer *mixer, t_obj_l *obj, t_vector *intersect);

t_vector	diffuse_get(t_mixer *mixer, t_diff diff, t_vector *result);

t_vector	rgbof_cast_vector(t_rgbof self);
t_rgbof		vector_cast_rgbof(t_vector self);

t_vector	trace_light(t_mixer *mixer, t_obj_l *curr, t_vector intersect);
t_vector	trace_rand(t_vector ray, t_vector normal, float diffusion);
bool		intersec_next(
				t_obj_l *objs,
				t_vector *origin,
				t_vector *ray,
				t_vector *inter);

t_rgbof		color_rgb(int r, int g, int b);
t_rgbof		color_add(t_rgbof first, t_rgbof second);
t_rgbof		color_add_cal(t_rgbof first, t_rgbof second);
t_rgbof		color_add_cal_cal(t_rgbof first, t_rgbof second);
t_rgbof		color_cal_rgb(t_rgbof color, float factor);
void		color_rgb_cal_result_mul(
				t_rgbof *res,
				t_rgbof color,
				float factor);
void		color_print(t_rgbof color);

bool		specular_highlight(
				t_vector *origin,
				t_obj_l *obj,
				t_vector *ray,
				t_vector *result);

/* MLX window handler */
int			key_redcross(t_mixer *p_null);
int			key_handler(int key, t_mixer *p_null);

/*				Lexer */
/* Validation */
int			validation_check(char **buffer, int size);
int			lexer(char *path, t_mixer *m_data, int *retval);
int			parser(char **buffer, t_mixer *m_data, int size);
void		correct_normals(t_mixer *aaaaaaaaaa);

/* Baseimage */
t_rgbof		sumup_light(t_mixer *mixer, t_col *c_s);

#endif /* MINIRT_H */
