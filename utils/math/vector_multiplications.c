#include <stddef.h>

#include "vector.h"

t_vector	*vector_new_multiply(t_vector *first, t_vector *second)
{
	t_vector	*ret;

	if (first == NULL || second == NULL)
		return (NULL);
	ret = vector_new(0, 0, 0);
	if (ret == NULL)
		return (NULL);
	vector_multiply(ret, first, second);
	return (ret);
}

void	vector_multiply(t_vector *result, t_vector *first, t_vector *second)
{
	if (result == NULL || first == NULL || second == NULL)
		return ;
	result->x = first->y * second->z - first->z * second->y;
	result->y = first->z * second->x - first->x * second->z;
	result->z = first->x * second->y - first->y * second->x;
}

float	vector_scalar_product(t_vector *one, t_vector *two)
{
	if (one == NULL || two == NULL)
		return (0);
	return (one->x * two->x + one->y * two->y + one->z * two->z);
}

t_vector	*vector_new_multiply_digit(t_vector *this, float multiplier)
{
	t_vector	*ret;

	if (this == NULL)
		return (NULL);
	ret = vector_new(0, 0, 0);
	if (ret == NULL)
		return (NULL);
	vector_multiply_digit(ret, this, multiplier);
	return (ret);
}

void	vector_multiply_digit(
			t_vector *result,
			t_vector *this,
			float multiplier)
{
	if (result == NULL || this == NULL)
		return ;
	result->x = this->x * multiplier;
	result->y = this->y * multiplier;
	result->z = this->z * multiplier;
}
