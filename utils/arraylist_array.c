/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arraylist_array.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:36:47 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/08 17:32:20 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "arraylist.h"

void	**arraylist_to_array(t_arraylist *this, t_arraylist_dup dup)
{
	void	**array;
	size_t	counter;

	if (dup == NULL)
		return (NULL);
	array = ft_gc_malloc(arraylist_size(this) * sizeof(void *));
	if (array == NULL)
		return (NULL);
	counter = 0;
	while (this != NULL)
	{
		array[counter] = dup(this->content);
		counter++;
		this = this->next;
	}
	return (array);
}

void	**arraylist_to_array_transfer(t_arraylist **this)
{
	void	**array;

	array = arraylist_to_array_transfer_core(*this);
	if (array != NULL)
		arraylist_clear(this, NULL);
	return (array);
}

void	**arraylist_to_array_transfer_core(t_arraylist *this)
{
	void	**array;
	size_t	counter;

	if (this == NULL)
		return (NULL);
	array = ft_gc_malloc(arraylist_size(this) * sizeof(void *));
	if (array == NULL)
		return (NULL);
	counter = 0;
	while (this != NULL)
	{
		array[counter] = this->content;
		counter++;
		this = this->next;
	}
	return (array);
}
