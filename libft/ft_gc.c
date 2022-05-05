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
#include <pthread.h>

static pthread_mutex_t	*ft_gc_mutex(void)
{
	static pthread_mutex_t	m;
	static bool				init = false;

	if (!init)
	{
		pthread_mutex_init(&m, NULL);
		init = true;
	}
	return (&m);
}

static t_arraylist	**ft_gc_list(void)
{
	static t_arraylist	*list = NULL;

	return (&list);
}

void	*ft_gc_malloc(size_t size)
{
	void	*ret;

	pthread_mutex_lock(ft_gc_mutex());
	ret = malloc(size);
	if (ret == NULL)
	{
		pthread_mutex_unlock(ft_gc_mutex());
		return (NULL);
	}
	if (!arraylist_append_unsafe(ft_gc_list(), arraylist_new(ret)))
	{
		free(ret);
		pthread_mutex_unlock(ft_gc_mutex());
		return (NULL);
	}
	pthread_mutex_unlock(ft_gc_mutex());
	return (ret);
}

void	ft_gc_free(void *ptr)
{
	t_arraylist	*findus;

	pthread_mutex_lock(ft_gc_mutex());
	findus = arraylist_find(*ft_gc_list(), ptr);
	arraylist_remove_at_element(ft_gc_list(), &findus, free);
	pthread_mutex_unlock(ft_gc_mutex());
}

//void	ft_gc_clear(void)
//{
//	pthread_mutex_lock(ft_gc_mutex());
//	arraylist_clear(ft_gc_list(), free);
//	*ft_gc_list() = NULL;
//	pthread_mutex_unlock(ft_gc_mutex());
//}

void	ft_gc_exit(int code)
{
	size_t	leak_count;

	pthread_mutex_lock(ft_gc_mutex());
	leak_count = arraylist_size_unsafe(*ft_gc_list());
	if (leak_count > 0)
		printf("%zu non-freed mallocs\n", leak_count);
	arraylist_clear(ft_gc_list(), free);
	pthread_mutex_unlock(ft_gc_mutex());
	pthread_mutex_destroy(ft_gc_mutex());
	exit(code);
}
