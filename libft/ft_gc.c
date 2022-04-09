/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 16:29:57 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/09 18:55:52 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/arraylist.h"

#include <stdio.h>
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

void	ft_gc_exit(int code)
{
	size_t	leak_count;

	leak_count = arraylist_size_unsafe(*ft_gc_list());
	if (leak_count > 0)
		printf("%zu non-freed mallocs\n", leak_count);
	arraylist_clear(ft_gc_list(), free);
	exit(code);
}
