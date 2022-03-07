#include <stdlib.h>

#include "matrix.h"
#include "model/vertex.h"

t_matrix	*matrix_new(
				float **matrix,
				unsigned int rows,
				unsigned int columns)
{
	t_matrix		*ret;
	unsigned int	i;

	ret = malloc(sizeof(struct s_matrix));
	if (ret == NULL)
		return (NULL);
	if (matrix == NULL)
	{
		matrix = malloc(rows * sizeof(float *));
		i = 0;
		while (i < rows)
			matrix[i++] = malloc(columns * sizeof(float));
		if (matrix == NULL)
			return (NULL);
	}
	matrix_create(ret, matrix, rows, columns);
	return (ret);
}

void	matrix_create(
			t_matrix *this,
			float **matrix,
			unsigned int rows,
			unsigned int columns)
{
	if (this != NULL)
	{
		this->values = matrix;
		this->rows = rows;
		this->columns = columns;
	}
}

void	matrix_cast_vertex3D(t_matrix *this, t_vertex3D *dest)
{
	if (this == NULL || dest == NULL
		|| (this->rows == 4 && this->columns != 1)
		|| (this->rows == 1 && this->columns != 4))
		return ;
	if (this->columns == 4)
	{
		vertex3D_create(dest, this->values[0][0], this->values[0][1],
			this->values[0][2]);
		dest->w = this->values[0][3];
	}
	else
	{
		vertex3D_create(dest, this->values[0][0], this->values[1][0],
			this->values[2][0]);
		dest->w = this->values[3][0];
	}
}

t_vertex3D	*matrix_new_cast_vertex3D(t_matrix *this)
{
	t_vertex3D	*ret;

	if (this == NULL
		|| (this->rows == 4 && this->columns != 1)
		|| (this->rows == 1 && this->columns != 4))
		return (NULL);
	if (this->columns == 4)
	{
		ret = vertex3D_new(this->values[0][0], this->values[0][1],
				this->values[0][2]);
		ret->w = this->values[0][3];
	}
	else
	{
		ret = vertex3D_new(this->values[0][0], this->values[1][0],
				this->values[2][0]);
		ret->w = this->values[3][0];
	}
	return (ret);
}
