/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:50:22 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/13 15:36:47 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdbool.h>
# include "vector.h"
# include "renderer_image.h"
# include "libft.h"

//Resolution in Pixel
# define RESOLUTION_X	400
# define RESOLUTION_Y	200
# define CAM_SIZE		100

//objecttypes
# define CAMERA		   -2
# define AMBIENT	   -1
# define LIGHT			0
# define SPHERE			1
# define PLANE			2
# define CYLINDER		3

# define LEXER_BUFFER	100
//red green blue values for an object or light 
//inits with white: 255, 255, 255
//RED GREEN BLUE OPACITY REFLECTIVITY
typedef struct s_rgbof {
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	o;
	unsigned char	f;
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

//camera struct 
//FOV between 0-180
typedef struct s_cam {
	t_vector	position;
	t_vector	normal;
	int			fov;
	float		aspect_ratio;
	t_vector	vecs[RESOLUTION_Y][RESOLUTION_X];
}	t_cam;

//mainstruct for MiniRT
typedef struct s_mixer {
	void				*p_mlx_init;
	void				*p_mlx_window;
	t_renderer_image	*image;

	t_obj_l				*obj_list;
	struct s_cam		cam;
	struct s_ambient	ambient;
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
bool	calc_intersection_sphere(t_cam cam, t_obj_l *obj, t_vector *ray, t_vector *ret);

/*
 * Calculates the intersecting vector for the given ray vector for the next
 * object in the given list.
 */
bool	calc_intersec_next(t_obj_l *objs, t_mixer *mixer, t_vector *ray, t_vector *ret);

/*
 * Returns wether the first intersecting vector is closer to the given camera
 * vector or the second one.
 */
bool	calc_intersec_dist(t_vector intersect, t_vector new_intersect, t_vector *cam);

/*
 * Calculates the first intersecting vector for the given ray vector and the
 * given object list.
 */
t_rgbof	calc_intersec_first(t_mixer *mixer, t_vector *ray, int y, int x);

/*
 * Calculates the intersecting vector for the given object and the camera
 * vector. The given object is expected to be a plane.
 */
bool	calc_intersection_plane(t_vector *cam, t_obj_l *objs, t_vector *ret);

/*
 * Returns wether the given vector intersects an infinite plane with the given
 * normal.
 */
bool	calc_intersecs_plane(t_vector *vec, t_vector *normal);

//				lexer
//validation
int		validation_check(char **buffer, int size);
int		lexer(char *path, t_mixer *m_data, int *retval);
int		parser(char **buffer, t_mixer *m_data, int size);
#endif /*MINIRT_H*/
