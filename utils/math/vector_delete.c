#include <stdlib.h>

#include "vector.h"

void	vector_destroy(t_vector __attribute__((unused)) *this)
{
}

void	vector_delete(t_vector *this)
{
	if (this != NULL)
		free(this);
}
