#ifndef MATRIX_H
# define MATRIX_H

# include <stdbool.h>

typedef struct s_vertex3D	t_vertex3D;

/*
 * Represents a mathematical matrix. Contains a two dimensional float array,
 * which is the actual matrix, as well as the length of the two dimensions.
 */
typedef struct s_matrix {
	unsigned int	rows;
	unsigned int	columns;
	float			**values;
}	t_matrix;

/*
 * Creates a new matrix object with the given values. If no matrix is given, a
 * new array will be allocated and filled with zeros. Returns the newly allocated
 * matrix object, null if the allocation failed.
 */
t_matrix	*matrix_new(
				float **matrix,
				unsigned int rows,
				unsigned int columns);

/*
 * Initializes the given matrix object using the given values. Does nothing if
 * no matrix object is given.
 */
void		matrix_create(
				t_matrix *this,
				float **matrix,
				unsigned int rows,
				unsigned int columns);

/*
 * Multiplies the given matrices and store the result in a newly allocated
 * matrix. Returns null if either the allocation failed or at least one
 * parameter is missing or if the calculation is impossible.
 */
t_matrix	*matrix_new_multiply(t_matrix *m1, t_matrix *m2);

/*
 * Creates a matrix filled with the given floating point number and the given
 * size. Returns the newly allocated matrix object, or null if the allocation
 * failed.
 */
t_matrix	*matrix_new_filled(
				float fill,
				unsigned int rows,
				unsigned int colummns);

/*
 * Allocates a new vertex, containing the values of the given matrix. Returns
 * either the newly allocated vertex, or null if the allocation failed, no
 * matrix is given or the matrix is not in the format 4x1 or 1x4.
 */
t_vertex3D	*matrix_cast_new_vertex3D(t_matrix *this);

/*
 * Writes the values of the given matrix into the given vertex. Does nothing if
 * either object is missing or the matrix is not in the format 4x1 or 1x4.
 */
void		matrix_cast_vertex3D(t_matrix *this, t_vertex3D *dest);

/*
 * Multiplies the given matrices and stores the result in the given matrix.
 * Does nothing if the given result matrix is not compatible to store the
 * result of the calculation. Does also nothing if at least one parameter is
 * missing. Overwrites the contents of the given result matrix.
 */
void		matrix_multiply(t_matrix *result, t_matrix *m1, t_matrix *m2);

/*
 * Multiplies the given matrix with the given vertex and stores the results in
 * the given vertex. Does nothing if at least one parameter is missing or the
 * given matrix is not in the format 4x4.
 */
void		matrix_multiply_vertex3D(
				t_vertex3D *result,
				t_matrix *m,
				t_vertex3D *v);

/*
 * Fills the given matrix with the given floating point number. Overwrites all
 * values that are already stored in the matrix! Does nothing if no matrix
 * object is given.
 */
void		matrix_fill(t_matrix *this, float fill);

/*
 * Fills the given matrix with values that make it neutral to multiplications.
 * Overwrites all values that are already stored in the matrix! Returns true if
 * the given matrix was filled, false otherwise.
 */
bool		matrix_fill_neutral(t_matrix *this);

/*
 * Destroys the given matrix object. Does not attempt to free the given object,
 * this method only destroys the contents of the given object. Does nothing if
 * no object is given.
 */
void		matrix_destroy(t_matrix *this);

/*
 * Deletes the given matrix object. Also frees the float array contained by the
 * given matrix object. Does nothing if no object is given.
 */
void		matrix_delete(t_matrix *this);

#endif
