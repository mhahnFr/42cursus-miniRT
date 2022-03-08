/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:50:22 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/08 15:22:03 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MINIRT_H_
#define _MINIRT_H_

#include "vector.h"

//Resolution in Pixel
#define RESOLUTION_X	800
#define RESOLUTION_Y	600

//objecttypes
#define LIGHT			0
#define SPHERE			1
#define PLANE			2
#define	CYLINDER		3


#define LEXER_BUFFER	100
//red green blue values for an object or light 
//inits with white: 255, 255, 255
//RED GREEN BLUE OPACITY REFLECTIVITY
typedef struct s_rgbof {
	char	R;
	char	G;
	char	B;
	char	O;
	char	F;
} t_rgbof;

//objectlist for all lights, and objects
typedef	struct s_object_l {
	int					obj_type;
	t_vector			position;
	double				width;		//in case of a sphere wifth and height are both the same
	double				height;		// in case of a plane we save the normal at the position and add the width and height here instead of building a plane from points.
	t_rgbof				color;		// will safe some work for calculating rays crossing the plane
	bool				emitter;
	float				brightness;

	struct s_object_l	*next;
	struct s_object_l	*prev;
} t_obj_l;

//mainstruct for MiniRT
typedef struct s_mixer {
	void*	p_mlx_init;
	void*	p_mlx_window;
	void*	image;

	t_obj_l	obj_list;
} t_mixer;

//functions


#endif /*_MINIRT_H_*/