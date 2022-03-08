#ifndef VERTEX_H
# define VERTEX_H

# include <stdbool.h>
# include "libft.h"

typedef struct s_matrix	t_matrix;

typedef struct s_point	t_point;

typedef struct s_vector	t_vector;

/*
 * Represents a three dimensional vertex.
 */
typedef struct s_vertex3d
{
	float			x;
	float			y;
	float			z;
	float			w;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	bool			colour_set;
}	t_vertex3d;

/*
 * Allocates a new vertex with the given coordinates. Returns the newly
 * allocated vertex, null in case of error.
 */
t_vertex3d	*vertex3d_new(float x, float y, float z);

/*
 * Initializes the content of the given vertex object using the given
 * parameters. Does nothing if no object is given.
 */
void		vertex3d_create(t_vertex3d *this, float x, float y, float z);

/*
 * Allocates a new vertex containing the values of the given vertex. Returns
 * either the copy of the given vertex or null if the allocation failed or no
 * vertex is given.
 */
t_vertex3d	*vertex3d_copy(t_vertex3d *this);

/*
 * Returns a newly allocated matrix, containing the values of the given vertex.
 * Returns either the newly allocated matrix, or null if the allocation failed
 * or no vertex is given.
 */
t_matrix	*vertex3d_cast_matrix(t_vertex3d *this);

/*
 * Returns a new point consisting of the X and Y values of the given vertex.
 * Returns either the newly allocated point or null if either the allocation
 * failed or no vertex is given.
 */
t_point		*vertex3d_cast_new_point(t_vertex3d *this);

/*
 * Stores the X and Y values of the given vertex in the given point. Does
 * nothing if at least one parameter is missing.
 */
void		vertex3d_cast_point(t_vertex3d *this, t_point *point);

/*
 * Allocates a new vector with the values of the given vertex. Returns either
 * the newly allocated vector or null if either the allocation failed or no
 * vertex is given.
 */
t_vector	*vertex3d_cast_new_vector(t_vertex3d *this);

/*
 * Copies the values of the given vertex into the given vector. Does nothing if
 * at least one parameter is missing.
 */
void		vertex3d_cast_vector(t_vertex3d *this, t_vector *vector);

/*
 * Copies the values of the given vertex in the other given vertex. Does
 * nothing if at least one vertex is missing.
 */
void		vertex3d_copy_values(t_vertex3d *this, t_vertex3d *new);

/*
 * Destroys the content of the given vertex object. Does nothing if no object
 * is given.
 */
void		vertex3d_destroy(t_vertex3d *this);

/*
 * Frees the given vertex.
 */
void		vertex3d_delete(t_vertex3d *this);

#endif
