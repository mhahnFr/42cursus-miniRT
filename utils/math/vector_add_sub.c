#include <stddef.h>

#include "vector.h"

t_vector	*vector_new_substract(t_vector *first, t_vector *second)
{
	t_vector	*ret;

	if (first == NULL || second == NULL)
		return (NULL);
	ret = vector_new(0, 0, 0);
	if (ret == NULL)
		return (NULL);
	vector_substract(ret, first, second);
	return (ret);
}

void	vector_substract(t_vector *result, t_vector *first, t_vector *second)
{
	if (result == NULL || first == NULL || second == NULL)
		return ;
	result->x = first->x - second->x;
	result->y = first->y - second->y;
	result->z = first->z - second->z;
}
