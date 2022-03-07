#ifndef MATRIX_NEW_H
# define MATRIX_NEW_H

/*
 * Represents a 3x3 matrix.
 */
typedef float	t_matrix3x3[3][3];

/*
 * Represents a 4x4 matrix.
 */
typedef float	t_matrix4x4[4][4];

/*
 * Represents a 3 dimension vector.
 */
typedef float	t_vec3[3];

/*
 * Represents a 4 dimensional vector.
 */
typedef float	t_vec4[4];

/*
 * Multiplies the given 4x4 matrix with the given 4 dimensional matrix.
 */
void	matrix_multiply_m4x4_v4(
			t_vec4 *result,
			t_matrix4x4 matrix,
			t_vec4 vector);

/*
 * Multiplies the given 4x4 matrices.
 */
void	matrix_multiply_m4x4_m4x4(
			t_matrix4x4 *result,
			t_matrix4x4 matrix1,
			t_matrix4x4 matrix2);

/*
 * Multiplies the given 3x3 matrices.
 */
void	matrix_multiply_m3x3_m3x3(
			t_matrix3x3 *result,
			t_matrix3x3 matrix1,
			t_matrix3x3 matrix2);

/*
 * Multiplies the given 3x3 matrix with the given 3 dimensional vector.
 */
void	matrix_multiply_m3x3_v3(
			t_vec3 *result,
			t_matrix3x3 matrix,
			t_vec3 vector);

#endif
