#include <stddef.h>

#include "matrix.h"

t_matrix	*matrix_new_filled(
				float fill,
				unsigned int rows,
				unsigned int columns)
{
	t_matrix	*ret;

	ret = matrix_new(NULL, rows, columns);
	if (ret != NULL)
		matrix_fill(ret, fill);
	return (ret);
}

void	matrix_fill(t_matrix *this, float fill)
{
	unsigned int	i;
	unsigned int	j;

	if (this != NULL)
	{
		i = 0;
		while (i < this->rows)
		{
			j = 0;
			while (j < this->columns)
			{
				this->values[i][j] = fill;
				j++;
			}
			i++;
		}
	}
}

bool	matrix_fill_neutral(t_matrix *this)
{
	unsigned int	i;

	if (this == NULL || this->rows != this->columns)
		return (false);
	matrix_fill(this, 0);
	i = 0;
	while (i < this->columns)
	{
		this->values[i][i] = 1;
		i++;
	}
	return (true);
}
