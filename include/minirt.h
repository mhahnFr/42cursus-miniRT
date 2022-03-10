/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:50:22 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/10 14:58:01 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdbool.h>
# include "vector.h"
# include "libft.h"

//Resolution in Pixel
# define RESOLUTION_X	800
# define RESOLUTION_Y	600

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
	char	r;
	char	g;
	char	b;
	char	o;
	char	f;
}	t_rgbof;

//objectlist for all lights, and objects
//
//WIDTH: in case of a sphere wifth and height are both the same
//HEIGHT-PLANE we save the normal at the position and add the width and height 
//				here instead of building a plane from points.
//NORMAL: will safe some work for calculating rays crossing the plane
typedef struct s_object_l {
	int					obj_type;
	t_vector			position;
	double				width;
	double				height;
	t_vector			normal;
	t_rgbof				color;		
	bool				emitter;
	float				brightness;

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
}	t_cam;

//mainstruct for MiniRT
typedef struct s_mixer {
	void				*p_mlx_init;
	void				*p_mlx_window;
	void				*image;

	t_obj_l				*obj_list;
	struct s_cam		cam;
	struct s_ambient	ambient;
}	t_mixer;

//functions

//				Main
/*
 * Creates the main delegate object. Takes the file to be opened as well as a
 * pointer to an integer in which to store the error code.
 */
t_mixer	*init_mainstruct(int *err);

//				Painter
/*
 * Paints the whole scene. Takes the delegate with all objects and
 * precalculated values as well as a pointer to an integer, in which the error
 * code is saved.
 */
void	paint(t_mixer *delegate, int *ret);

//				lexer
//validation
int		validation_check(char **buffer, int size);
int		lexer(char *path, t_mixer *m_data, int *retval);
int		parser(char **buffer, t_mixer *m_data, int size);
#endif /*MINIRT_H*/
