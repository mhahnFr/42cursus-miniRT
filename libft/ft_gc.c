/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 16:29:57 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/06 12:59:51 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/arraylist.h"

#include <stdlib.h>
#include <pthread.h>

static pthread_mutex_t	*ft_gc_get_mutex(void)
{
	static bool				inited = false;
	static pthread_mutex_t	mutex;

	if (!inited)
	{
		pthread_mutex_init(&mutex, NULL);
		inited = true;
	}
	return (&mutex);
}

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
	pthread_mutex_lock(ft_gc_get_mutex());
	if (!arraylist_append_unsafe(ft_gc_list(), arraylist_new(ret)))
	{
		pthread_mutex_unlock(ft_gc_get_mutex());
		free(ret);
		return (NULL);
	}
	pthread_mutex_unlock(ft_gc_get_mutex());
	return (ret);
}

void	ft_gc_free(void *ptr)
{
	t_arraylist	*findus;

	pthread_mutex_lock(ft_gc_get_mutex());
	findus = arraylist_find(*ft_gc_list(), ptr);
	arraylist_remove_at_element(ft_gc_list(), &findus, free);
	pthread_mutex_unlock(ft_gc_get_mutex());
}

void	ft_gc_clear(void)
{
	pthread_mutex_lock(ft_gc_get_mutex());
	arraylist_clear(ft_gc_list(), free);
	*ft_gc_list() = NULL;
	pthread_mutex_unlock(ft_gc_get_mutex());
}

#ifdef DEBUG
#include <stdio.h>
#endif
void	ft_gc_exit(int code)
{
	pthread_mutex_lock(ft_gc_get_mutex());
#ifdef DEBUG
	size_t leak_count = arraylist_size_unsafe(*ft_gc_list());
	printf("%zu leaks\n", leak_count);
#endif
	arraylist_clear(ft_gc_list(), free);
	pthread_mutex_unlock(ft_gc_get_mutex());
	pthread_mutex_destroy(ft_gc_get_mutex());
	exit(code);
}
