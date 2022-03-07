#include "matrix_new.h"

void	matrix_multiply_m4x4_v4(t_vec4 *result, t_matrix4x4 m, t_vec4 v)
{
	int	i;
	int	j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			(*result)[i] += m[i][j] * v[j];
			j++;
		}
		i++;
	}
}

void	matrix_multiply_m4x4_m4x4(
			t_matrix4x4 *result,
			t_matrix4x4 m1,
			t_matrix4x4 m2)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			k = 0;
			while (k < 4)
			{
				(*result)[i][j] += m1[i][k] * m2[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
}

void	matrix_multiply_m3x3_v3(t_vec3 *result, t_matrix3x3 m, t_vec3 v)
{
	int	i;
	int	j;

	i = 0;
	while (i < 3)
	{
		j = 0;
		while (j < 3)
		{
			(*result)[i] += m[i][j] * v[j];
			j++;
		}
		i++;
	}
}

/*void	matrix_multiply_mAxA_vA(
			t_vec *result,
			t_matrix m,
			t_vec v,
			unsigned int length)
{
	for (unsigned int i = 0; i < length; i++)
		for (unsigned int j = 0; j < length; j++)
			(*result)[i] += m[i][j] * v[j];
}*/

void	matrix_multiply_m3x3_m3x3(
			t_matrix3x3 *result,
			t_matrix3x3 m1,
			t_matrix3x3 m2)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (i < 3)
	{
		j = 0;
		while (j < 3)
		{
			k = 0;
			while (k < 3)
			{
				(*result)[i][j] += m1[i][k] * m2[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
}

/*void	matrix_multiply_mAxA_mAxA(
			t_matrix *result,
			t_matrix m1,
			t_matrix m2,
			unsigned int length)
{
	for (unsigned int i = 0; i < length; i++) {
		for (unsigned int j = 0; j < length; j++) {
			for (unsigned int k = 0; k < length; k++) {
				(*result)[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}
}*/
