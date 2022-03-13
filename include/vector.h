/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:38:23 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/13 15:27:44 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include "libft.h"

/*
 * Represents a vector with three dimensions.
 */
typedef struct s_vector
{
	float	x;
	float	y;
	float	z;
}	t_vector;

/*
 * Allocates a new vector with the given values. Returns either the newly
 * allocated vector or null if the allocation failed.
 */
t_vector	*vector_new(float x, float y, float z);

/*
 * Initializes the given vector with the given values. Does nothing if no
 * vector is given.
 */
void		vector_create(t_vector *this, float x, float y, float z);

/*
 * Allocates a new vector containing the result of the substraction of the
 * second vector from the first one. Return either the newly allocated vector
 * or null if the allocation failed or at least one vector is missing.
 */
t_vector	*vector_new_substract(t_vector *first, t_vector *second);

/*
 * Substract the two given vectors and stores the result in the given vector.
 * Does nothing if at least one vector is missing.
 */
void		vector_substract(
				t_vector *result,
				t_vector *first,
				t_vector *second);

/*
 * Adds the two given vectors and stores the result in the given vector.
 * Does nothing if at least one vector is missing.
 */
void		vector_addition(
				t_vector *result,
				t_vector *first,
				t_vector *second);

/*
 * Allocates a new vector containing the result of the addition of the first
 * vector to the second one. Return either the newly allocated vector or null
 * if the allocation failed or at least one vector is missing.
 */
t_vector	*vector_new_addition(t_vector *first, t_vector *second);

/*
 * Calculates and returns the length of the given vector. Returns zero if no
 * vector is given.
 */
float		vector_length(t_vector *this);

/*
 * Multiplies the given vector with the given number. Returns either the newly
 * allocated vector or null if either the allocation failed or no vector is
 * given.
 */
t_vector	*vector_new_multiply_digit(t_vector *this, float multiplicator);

/*
 * Multiplies the given vector with the given number and stores the result in
 * the given vector. Does nothing if at least one vector is missing.
 */
void		vector_multiply_digit(
				t_vector *result,
				t_vector *this,
				float multiplicator);

/*
 * Calculates the distance between the two given vectors. Returns zero, if at
 * least one vector is missing. Otherwise, the calculated distance is returned.
 */
float		vector_distance(t_vector *first, t_vector *second);

/*
 * Normalizes the given vector. Does nothing if no vector is given.
 * NOTE: Does not work for vectors with a length of zero.
 */
void		vector_normalize(t_vector *this);

/*
 * Performs a division with the given vector and stores the result in the
 * given vector. Does nothing if at least one vector is missing.
 */
void		vector_division(
				t_vector *result,
				t_vector *divident,
				float divisor);

/*
 * Performs a division on the two given vectors and returns a newly allocated
 * vector containing the result of the division. Returns either the newly
 * allocated vector or null of the allocation failed or at least one vector is
 * missing.
 */
t_vector	*vector_new_division(t_vector *divident, float divisor);

/*
 * Calculates the cross product of the two given vectors and stores the result
 * in a newly allocated vector, which is returned. Returns null if the
 * allocation failed or at least one vector is missing. Otherwise, the vector
 * containing the result is returned.
 */
t_vector	*vector_new_cross_product(t_vector *first, t_vector *second);

/*
 * Calculates the cross product of the to given vectors and stores the result
 * in the given vector. Does nothing if at least one vector is missing.
 */
void		vector_cross_product(
				t_vector *result,
				t_vector *first,
				t_vector *second);

/*
 * Multiplies the two given vectors and returns the result as a newly allocated
 * vector. Returns either the newly allocated vector or null if the allocation
 * failed or at least one vector is missing.
 */
t_vector	*vector_new_multiply(t_vector *first, t_vector *second);

/*
 * Multiplies the two given vectors and stores the result in the given vector.
 * Does nothing if at least one vector is missing.
 */
void		vector_multiply(
				t_vector *result,
				t_vector *first,
				t_vector *second);

/*
 * Calculates the scalar product of the given vectors. Returns either the
 * calculated result or zero, if at least one vector is missing.
 */
float		vector_scalar_product(t_vector *first, t_vector *second);

/*
 * Destroys the given vector. Does nothing if no vector is given.
 */
void		vector_destroy(t_vector *this);

/*
 * Deletes the given vector. Does nothing if no vector is given.
 */
void		vector_delete(t_vector *this);

#endif
