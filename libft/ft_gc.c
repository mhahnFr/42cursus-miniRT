#include "../include/arraylist.h"

#include <stdlib.h>

static t_arraylist	**ft_gc_list(void)
{
	static t_arraylist	*list = NULL;
	return (&list);
}

void	*ft_gc_malloc(size_t size)
{
	void	*ret;

	ret = malloc(size);
	if (ret == NULL)
		return (NULL);
	if (!arraylist_append_unsafe(ft_gc_list(), arraylist_new(ret)))
	{
		free(ret);
		return (NULL);
	}
	return (ret);
}

void	ft_gc_free(void *ptr)
{
	t_arraylist	*findus;

	findus = arraylist_find(*ft_gc_list(), ptr);
	arraylist_remove_at_element(ft_gc_list(), &findus, free);
}

void	ft_gc_clear(void)
{
	arraylist_clear(ft_gc_list(), free);
	*ft_gc_list() = NULL;
}

#ifdef DEBUG
#include <stdio.h>
#endif
void	ft_gc_exit(int code)
{
#ifdef DEBUG
	size_t leak_count = arraylist_size_unsafe(*ft_gc_list());
	if (leak_count > 0) {
		printf("%zu leaks\n", leak_count);
	} else {
		printf("All heap blocks were freed - NO LEAKS ARE POSSIBLE\n");
	}
#endif
	arraylist_clear(ft_gc_list(), free);
	exit(code);
}
