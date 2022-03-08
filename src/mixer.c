#include <stdlib.h>

#include "minirt.h"

t_mixer	*init_mainstruct(int *err)
{
	t_mixer	*ret;

	ret = malloc(sizeof(struct s_mixer));
	if (ret == NULL)
	{
		*err = 3;
		return (NULL);
	}
	return (ret);
}
