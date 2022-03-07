#include <stdlib.h>

#include "point.h"

t_point	*point_new(long x, long y)
{
	t_point	*ret;

	ret = malloc(sizeof(struct s_point));
	if (ret == NULL)
		return (NULL);
	point_create(ret, x, y);
	return (ret);
}

void	point_create(t_point *this, long x, long y)
{
	if (this == NULL)
		return ;
	this->x = x;
	this->y = y;
	this->r = 1;
	this->g = 1;
	this->b = 1;
}

void	point_destroy(t_point *this)
{
	if (this == NULL)
		return ;
}

void	point_delete(t_point *this)
{
	if (this == NULL)
		return ;
	free(this);
}
