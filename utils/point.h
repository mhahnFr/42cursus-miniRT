#ifndef POINT_H
# define POINT_H

/*
 * Represents a two dimensional point. Consists of the X and the Y value and
 * the colour values.
 */
typedef struct s_point {
	long			x;
	long			y;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}	t_point;

/*
 * Allocates a new point with the given values. Returns either the newly
 * allocated point or null if the allocation failed.
 */
t_point	*point_new(long x, long y);

/*
 * Initializes the given point using the given values. Does nothing if no point
 * is given.
 */
void	point_create(t_point *this, long x, long y);

/*
 * Copies the values of the given point and stores them in the given point.
 * Does nothing if at least one point is missing.
 */
void	point_copy_values(t_point *this, t_point *copy);

/*
 * Allocates a new point and copies the values of the given point. Returns
 * either the newly allocated point or null if either the allocation failed or
 * no point is given.
 */
t_point	*point_copy(t_point *this);

/*
 * Destroys the contents of the given point. Does nothing if no point is given.
 */
void	point_destroy(t_point *this);

/*
 * Deletes the given point. Does nothing if no point is given.
 */
void	point_delete(t_point *this);

#endif
