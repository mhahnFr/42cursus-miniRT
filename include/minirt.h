/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:50:22 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/07 11:58:23 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <pthread.h>
# include <stdbool.h>

# include "libft.h"
# include "renderer_image.h"
# include "vector.h"

/* Resolution in pixels */
# define RESOLUTION_X	480
# define RESOLUTION_Y	320

# define ANTI_ALIASING	10
# define MAX_BOUNCES	20
# define BLOCK_SIZE		40

/* Object types */
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
 * red green blue values for an object or light 
 * inits with white: 255, 255, 255
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
 * Objectlist for all lights, and objects
 *
 * WIDTH: in case of a sphere width and height are both the same
 */
typedef struct s_object_l {
	bool				emitter;
	double				height;
	double				width;
	float				brightness;
	float				diffusion;
	float				intensity;
	float				reflec_fac;
	int					obj_type;
	t_rgbof				color;
	t_vector			normal;
	t_vector			position;

	struct s_object_l	*next;
	struct s_object_l	*prev;
}	t_obj_l;

/*
 * Represents the ambient colour. Consists of the actual colour and the
 * brightness.
 */
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
 * FOV between 0-180 degrees
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

/*
 * color_calcstruct
 */
typedef struct s_col_calc
{
	bool		sw;
	float		*fac;
	int			l_count;
	t_vector	*sum;
	t_vector	basecolor;
	t_vector	diff;
}	t_col;

typedef struct s_thread	t_thread;

/*
 * Tile list for multicore rendering pipeline with better performance
 */
typedef struct s_tile {
	int				x;
	int				y;
	pthread_mutex_t	m_rendered;
	bool			rendered;
}	t_tile;

/*
 * Mainstruct for MiniRT
 */
typedef struct s_mixer {
	t_renderer_image	*image;
	void				*p_mlx_init;
	void				*p_mlx_window;

	int					light_count;
	size_t				cores;
	struct s_ambient	ambient;
	struct s_cam		cam;
	t_tile				**tile_array;
	t_obj_l				*obj_list;
	t_thread			*threads;
}	t_mixer;

/*
 * Thread local variables.
 */
typedef struct s_thread {
	float				disthit;
	int					bounces;
	pthread_t			thread;
//	size_t				block_size_x;
	size_t				index;
	int					x;
	int					y;
	struct s_col_calc	col_sum;
	t_diff				diff_sh;
	t_mixer				*mixer;
	t_vector			obj_int_normal;
}	t_thread;

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
void		paint(t_mixer *delegate, int *ret);

/*				Calculator */
void		calculator(t_mixer *mixer, int *ret);

void		calc_object_ray(t_mixer *mixer, int *ret);
void		skip_obj(t_obj_l **objs, int toskip);

bool		hit_sphere(
				t_thread *self,
				t_vector *origin,
				t_obj_l *obj,
				t_vector *ray,
				t_vector *ret);

bool		calc_intersec_next(
				t_obj_l *objs,
				t_vector *mixer,
				t_vector *ray,
				t_vector *ret);

bool		calc_intersec_dist(
				t_vector intersect,
				t_vector new_intersect,
				t_vector *cam);

t_rgbof		calc_random_rays(t_mixer *mixer, t_vector *ray, int y, int x);

t_rgbof		calc_intersec_first(t_mixer *mixer, t_vector *ray, t_rgbof pcolor);

bool		intersec_plane(
				t_thread *self,
				t_vector *cam,
				t_vector *cam_pos,
				t_obj_l *objs,
				t_vector *ret);

bool		fast_intersec_plane(t_vector *vec, t_vector *normal);

t_rgbof		calc_shader(
				t_vector *origin,
				t_vector *ray,
				t_thread *self,
				t_col *colsum);

t_rgbof		calc_antialiasing(t_thread *self, t_vector *cam_vec);

t_vector	diffuse_main(t_thread *self, t_obj_l *obj, t_vector *intersect);

t_vector	diffuse_get(t_thread *self, t_diff diff, t_vector *result);

t_vector	rgbof_cast_vector(t_rgbof self);
t_rgbof		vector_cast_rgbof(t_vector self);

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

void		rt_start(t_mixer *mixer);
void		draw_point(int x, int y, t_renderer_image *buffer, t_rgbof colour);
bool		specular_highlight(
				t_thread *self,
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

#endif /* MINIRT_H */
