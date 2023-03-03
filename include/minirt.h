/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:50:22 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/26 18:44:02 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

#ifdef WINDOWS
# define printf __mingw_printf
#endif

# include <stdbool.h>
# include <pthread.h>
# include "vector.h"
# include "renderer_image.h"
# include "libft.h"
# include "MLX42/MLX42.h"

/* Resolution in Pixel */
/* Object-types */
# define CAMERA		   -2
# define AMBIENT	   -1
# define LIGHT			0
# define SPHERE			1
# define PLANE			2
# define START			3
# define CYLINDER_CAP	4
# define CYLINDER_MAN	5
# define CUBE_PLANE		6

# define LEXER_BUFFER	100
# define ESC_KEY		53
# define BLOCK_SIZE		40

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
	float				depth;
	float				brightness;
	float				diffusion;
	float				disthit;
	float				intensity;
	float				reflec_fac;
	int					obj_type;
	int					cube_side;
	t_rgbof				color;
	t_vector			col_normal;
	t_vector			normal;
	t_vector			normal_left;
	t_vector			normal_down;
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
	t_vector	ey;
	t_vector	normal;
	t_vector	position;
	t_vector	step;
	t_vector	**vecs;
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

typedef struct s_tile {
	pthread_mutex_t	m_rendered;
	int32_t			x;
	int32_t			y;
	bool			rendered;
}	t_tile;

struct	s_mixer;

typedef struct s_threads {
	struct s_mixer		*mixer;
	size_t				index;
	pthread_t			thread;
}	t_thread;

/* Mainstruct for MiniRT */
typedef struct s_mixer {
	t_renderer_image	*image;
//	void				*p_mlx_init;
//	void				*p_mlx_window;
	mlx_t				*mlx;

	int32_t				res_x;
	int32_t				res_y;
	size_t				antialiasing;
	int					bounces;
	int					max_bounces;
	int					light_count;
	struct s_ambient	ambient;
	struct s_cam		cam;
	struct s_col_calc	col_sum;
	t_diff				diff_sh;
	t_obj_l				*obj_list;
	size_t				cores;
	t_tile				**tile_array;
	struct s_threads	*threads;
}	t_mixer;

//needed for calculator, for norm reason
typedef struct s_iobj {
	t_vector	ray;
	t_vector	origin;
	t_vector	inter;
	t_vector	ret_color;
	bool		shadow;
	float		length;
	t_obj_l		*list;
	t_obj_l		*curr;
	t_obj_l		*light;
	t_obj_l		*obj_col;
	t_vector	stack_vecs[4];
}	t_iobj;

typedef struct s_anti_norm {
	float		a;
	float		b;
	float		c;
	float		d;
	float		temp;
	t_vector	oc;
	t_vector	*origin;
	t_vector	*ray;
	t_vector	*ret;
	t_obj_l		*obj;
}	t_anti_norm;

typedef struct s_test {
	float		a;
	float		b;
	float		c;
	float		d;
	float		root;
	t_vector	*origin;
	t_vector	*ray;
	t_vector	*inter;
	t_obj_l		*obj;
}	t_test;

/* Functions */

/*				Main */
/*
 * Creates the main delegate object. Takes a pointer to an integer in which to
 * store the error code.
 */
t_mixer		*init_mainstruct(int *err);
void		print_help(char *bad_arg);
bool		string_equals(const char *str1, const char *str2);
void		print_error_cli(const char *arg);
void		init_mixer_image(t_mixer *self);
void		set_res(t_mixer *self, char *width, char *height);
void		set_antialiasing(t_mixer *self, char *factor);
void		print_values(t_mixer *self);
bool		string_is_digits(const char *str);
void		set_max_bounces(t_mixer *self, char *bounces);
char		*config_mixer(t_mixer *self, char **argv, int argc, int *ret);
void		rt_start(t_mixer *mixer);
void		free_mixer_copy(t_mixer *self);
/*
 * Initializes the values of the camera. Takes the mixer object as parameter.
 */
void		rt_cam(t_mixer *mixer);

/*				Painter */
void		draw_point(size_t x, size_t y, t_renderer_image *i, t_rgbof c);
t_mixer		*copy_mixer(t_mixer *self);
void		rt_runner(t_thread *self);

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

bool		hit_cube(
				t_vector *origin,
				t_obj_l *objs,
				t_vector *ray,
				t_vector *inter);

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

bool		cylinder_length_check(t_obj_l *self, t_vector *sect);

t_vector	cylinder_intersect_normal(
				t_vector *origin,
				t_vector *inter,
				t_vector *normal,
				bool inv);

bool		hit_cylinder_cap(
				t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *sect);

bool		hit_cylinder_mantel(t_vector *origin,
				t_obj_l *obj,
				t_vector *ray,
				t_vector *inter);

bool		hit_cylinder_part_b_a(t_test cy_struct);

t_rgbof		calc_shader(
				t_vector *origin,
				t_vector *ray,
				t_mixer *mixer,
				t_col *colsum);

t_rgbof		calc_antialiasing(
				t_mixer *mixer,
				t_vector *cam_vec,
				t_rgbof color);

float		cylinder_part_c(
				t_vector p, t_vector p_c, t_vector d_c, float radius);

float		cylinder_part_b(
				t_vector *d, t_vector *d_c, t_vector *p, t_vector *pc);

float		cylinder_part_a(t_vector *d, t_vector *d_c);

t_rgbof		calc_first_ray_of_the_day(t_mixer *mixer, t_vector *cam_vec);

t_vector	diffuse_main(t_mixer *mixer, t_obj_l *obj, t_vector *intersect);

t_vector	diffuse_get(t_mixer *mixer, t_diff diff, t_vector *result);

t_vector	rgbof_cast_vector(t_rgbof self);
t_rgbof		vector_cast_rgbof(t_vector self);

t_vector	trace_light(t_mixer *mixer, t_obj_l *curr, t_vector *vecs[2]);
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

#ifdef WINDOWS
void		key_redcross(void *p_null);
#else
void		key_redcross(t_mixer *p_null);
#endif

#ifdef WINDOWS
void key_handler(int key, void *p_null);
#else
void key_handler(int key, t_mixer *p_null);
#endif

/*				Lexer */
/* Validation */
int			validation_check(char **buffer, int size);
int			lexer(char *path, t_mixer *m_data, int *retval);
int			parser(char **buffer, t_mixer *m_data, int size);
void		correct_normals(t_mixer *mixer);

#endif /* MINIRT_H */
