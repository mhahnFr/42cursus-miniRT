#include "minirt.h"

t_delegate	*init_mainstruct(char *file, int *ret)
{
	t_delegate	*ret;

	ret = malloc(sizeof(struct s_delegate));
	if (ret == NULL)
		return (NULL);
	return (ret);
}
