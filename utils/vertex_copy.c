#include <stddef.h>

#include "vertex.h"

t_vertex3d	*vertex3d_copy(t_vertex3d *this)
{
	if (this == NULL)
		return (NULL);
	return (vertex3d_new(this->x, this->y, this->z));
}

void	vertex3d_copy_values(t_vertex3d *this, t_vertex3d *new)
{
	if (this == NULL || new == NULL)
		return ;
	vertex3d_create(new, this->x, this->y, this->z);
}
