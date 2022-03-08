#include <stdlib.h>

#include "minirt.h"

t_delegate	*init_mainstruct(char *file, int *err)
{
	t_delegate	*ret;

	(void) file;
	ret = malloc(sizeof(struct s_delegate));
	if (ret == NULL)
	{
		*err = 3;
		return (NULL);
	}
	return (ret);
}
