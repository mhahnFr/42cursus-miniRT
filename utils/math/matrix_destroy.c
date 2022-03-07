#include <stdlib.h>

#include "matrix.h"

void	matrix_destroy(t_matrix __attribute__((unused)) *this)
{
}

void	matrix_delete(t_matrix *this)
{
	unsigned int	i;

	matrix_destroy(this);
	if (this != NULL)
	{
		if (this->values != NULL)
		{
			i = 0;
			while (i < this->rows)
			{
				free(this->values[i]);
				i++;
			}
			free(this->values);
		}
		free(this);
	}
}
