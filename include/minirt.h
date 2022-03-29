/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:50:22 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/29 15:54:53 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdbool.h>
# include "vector.h"
# include "renderer_image.h"
# include "libft.h"

//Resolution in Pixel
# define RESOLUTION_X	1080
# define RESOLUTION_Y	720
//# define CAM_SIZE		10
# define ANTI_ALIASING	5
# define MAX_BOUNCES	10
//objecttypes
# define CAMERA		   -2
# define AMBIENT	   -1
# define LIGHT			0
# define SPHERE			1
# define PLANE			2
# define CYLINDER		3

# define LEXER_BUFFER	100
# define ESC_KEY		53
//red green blue values for an object or light 
//inits with white: 255, 255, 255
//RED GREEN BLUE OPACITY REFLECTIVITY
typedef struct s_rgbof {
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	o;
	unsigned char	f;

	unsigned int				cal_r;
	unsigned int				cal_g;
	unsigned int				cal_b;
}	t_rgbof;
//objectlist for all lights, and objects
//
//WIDTH: in case of a sphere wifth and height are both the same
typedef struct s_object_l {
	int					obj_type;
	t_vector			position;
	double				width;
	double				height;
	t_vector			normal;
	t_rgbof				color;		
	bool				emitter;
	float				brightness;
	t_vector			col_normal;
	float				disthit;

	struct s_object_l	*next;
	struct s_object_l	*prev;
}	t_obj_l;

typedef struct s_ambient {
	float	a_light;
	t_rgbof	color;
}	t_ambient;

typedef struct s_diffuse
{
	t_obj_l		*hit;
	t_vector	*ray;
	t_vector	*origin;
	int			ray_count;
}	t_diff;

//camera struct 
//FOV between 0-180
typedef struct s_cam {
	t_vector	position;
	t_vector	normal;
	int			fov;
	float		aspect_ratio;
	t_vector	vecs[RESOLUTION_Y][RESOLUTION_X];
	t_vector	step;
	t_vector	hori;
	t_vector	vert;
	t_vector	llc;
}	t_cam;

//mainstruct for MiniRT
typedef struct s_mixer {
	void				*p_mlx_init;
	void				*p_mlx_window;
	t_renderer_image	*image;

	t_obj_l				*obj_list;
	struct s_cam		cam;
	struct s_ambient	ambient;
	t_diff				diff_sh;
}	t_mixer;

//functions

//				Main
/*
 * Creates the main delegate object. Takes a pointer to an integer in which to
 * store the error code.
 */
t_mixer	*init_mainstruct(int *err);

/*
 * Initializes the values of the camera. Takes the mixer object as parameter.
 */
void	rt_cam_init(t_mixer *mixer);

//				Painter
/*
 * Paints the whole scene. Takes the delegate with all objects and
 * precalculated values as well as a pointer to an integer, in which the error
 * code is saved.
 */
void	paint(t_mixer *delegate, int *ret);

//				Calculator
void	calculator(t_mixer *mixer, int *ret);

/*
 * Iterates over all vectors of the camera stored in the given mixer object and
 * calculates their intersecting vectors, if there are any.
 */
void	calc_object_ray(t_mixer *mixer, int *ret);
/*
 * Skips the objects of the given type at the beginning of the given list.
 */
void	skip_obj(t_obj_l **objs, int toskip);

/*
 * Calculates the intersecting vector of the given object, which is treated as
 * a sphere. Takes the camera object, the sphere object and the ray for which
 * to calculate the intersecting vector.
 */
bool	hit_sphere(t_vector	*origin, t_obj_l *obj, t_vector *ray, t_vector *ret);

/*
 * Calculates the intersecting vector for the given ray vector for the next
 * object in the given list.
 */
bool	calc_intersec_next(t_obj_l *objs, t_vector *mixer, t_vector *ray, t_vector *ret);

/*
 * Returns wether the first intersecting vector is closer to the given camera
 * vector or the second one.
 */
bool	calc_intersec_dist(t_vector intersect, t_vector new_intersect, t_vector *cam);

t_rgbof	calc_random_rays(t_mixer *mixer, t_vector *ray, int y, int x);

/*
 * Calculates the first intersecting vector for the given ray vector and the
 * given object list.
 */
t_rgbof	calc_intersec_first(t_mixer *mixer, t_vector *ray, t_rgbof pcolor);

/*
 * Calculates the intersecting vector for the given object and the camera
 * vector. The given object is expected to be a plane.
 */
bool	intersec_plane(t_vector *cam, t_vector*, t_obj_l *objs, t_vector *ret);

/*
 * Returns wether the given vector intersects an infinite plane with the given
 * normal.
 */
bool	fast_intersec_plane(t_vector *vec, t_vector *normal);

t_rgbof	calc_shader(t_vector *origin, t_vector *ray, t_mixer *mixer);

t_rgbof	calc_antialiasing(t_mixer *mixer, t_vector *cam_vec, int y, int x);

t_rgbof	diffuse_main(t_mixer *mixer, t_obj_l *obj, t_vector *intersect);

t_vector	diffuse_get(t_mixer *mixer, t_diff diff, t_vector *result);

t_vector	rgbof_cast_vector(t_rgbof self);
t_rgbof		vector_cast_rgbof(t_vector self);

t_rgbof	color_rgb(int r, int g, int b);
t_rgbof	color_add(t_rgbof first, t_rgbof second);
t_rgbof	color_add_cal(t_rgbof first, t_rgbof second);
t_rgbof	color_add_cal_cal(t_rgbof first, t_rgbof second);
t_rgbof	color_cal_rgb(t_rgbof color, float factor);
void	color_rgb_cal_result_mul(t_rgbof *res, t_rgbof color, float factor);
void	color_print(t_rgbof color);

//MLX window handler
int		key_redcross(t_mixer *p_null);
int		key_handler(int key, t_mixer *p_null);
//				lexer
//validation
int		validation_check(char **buffer, int size);
int		lexer(char *path, t_mixer *m_data, int *retval);
int		parser(char **buffer, t_mixer *m_data, int size);
#endif /*MINIRT_H*/
