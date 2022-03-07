#include <stddef.h>

#include "libft.h"

#include "matrix.h"
#include "model/vertex.h"

t_matrix	*matrix_new_multiply(t_matrix *m1, t_matrix *m2)
{
	t_matrix		*result;

	if (m1 == NULL || m2 == NULL || m1->columns != m2->rows)
		return (NULL);
	result = matrix_new(NULL, m1->rows, m2->columns);
	if (result == NULL)
		return (NULL);
	matrix_fill(result, 0);
	matrix_multiply(result, m1, m2);
	return (result);
}

void	matrix_multiply(t_matrix *result, t_matrix *m1, t_matrix *m2)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	k;

	if (result == NULL || m1 == NULL || m2 == NULL || m1->columns != m2->rows
		|| result->rows != m1->rows || result->columns != m2->columns)
		return ;
	i = 0;
	while (i < m1->rows)
	{
		j = 0;
		while (j < m2->columns)
		{
			k = 0;
			while (k < m1->columns)
			{
				result->values[i][j] += m1->values[i][k] * m2->values[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
}

void	matrix_multiply_vertex3D(
			t_vertex3D *result,
			t_matrix *m,
			t_vertex3D *v)
{
	if (result == NULL || m == NULL || v == NULL
		|| m->rows != 4 || m->columns != 4)
		return ;
	result->x = m->values[0][0] * v->x + m->values[0][1] * v->y
		+ m->values[0][2] * v->z + m->values[0][3] * v->w;
	result->y = m->values[1][0] * v->x + m->values[1][1] * v->y
		+ m->values[1][2] * v->z + m->values[1][3] * v->w;
	result->z = m->values[2][0] * v->x + m->values[2][1] * v->y
		+ m->values[2][2] * v->z + m->values[2][3] * v->w;
	result->w = m->values[3][0] * v->x + m->values[3][1] * v->y
		+ m->values[3][2] * v->z + m->values[3][3] * v->w;
	result->r = v->r;
	result->g = v->g;
	result->b = v->b;
	result->colour_set = v->colour_set;
}
