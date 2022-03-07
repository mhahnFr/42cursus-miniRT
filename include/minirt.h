/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:50:22 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/07 18:35:50 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MINIRT_H_
#define _MINIRT_H_

#include "vector.h"

//Resolution in Pixel
#define RESOLUTION_X	800
#define RESOLUTION_Y	600

//objecttypes
#define LIGHT		0
#define SPHERE		1
#define PLANE		2
#define	CYLINDER	3

//red green blue values for an object or light 
//inits with white: 255, 255, 255
typedef struct s_rgb {
	char	R;
	char	G;
	char	B;
} t_rgb;

//objectlist for all lights, and objects
typedef	struct s_object_l {
	int					obj_type;
	t_vector			position;
	double				width;		//in case of a sphere wifth and height are both the same
	double				height;
	t_rgb				color;

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