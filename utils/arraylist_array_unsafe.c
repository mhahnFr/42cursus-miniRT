/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arraylist_array_unsafe.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:36:40 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/08 17:36:30 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "arraylist.h"

void	**arraylist_to_array_unsafe(t_arraylist *this, t_arraylist_dup dup)
{
	void	**array;

	if (dup == NULL)
		return (NULL);
	array = malloc(arraylist_size_unsafe(this) * sizeof(void *));
	if (array == NULL)
		return (NULL);
	while (this != NULL)
	{
		array[this->index] = dup(this->content);
		this = this->next;
	}
	return (array);
}

void	**arraylist_to_array_transfer_unsafe(t_arraylist **this)
{
	void	**array;

	array = arraylist_to_array_transfer_core_unsafe(*this);
	if (array != NULL)
		arraylist_clear(this, NULL);
	return (array);
}

void	**arraylist_to_array_transfer_core_unsafe(t_arraylist *this)
{
	void	**array;

	if (this == NULL)
		return (NULL);
	array = malloc(arraylist_size_unsafe(this) * sizeof(void *));
	if (array == NULL)
		return (NULL);
	while (this != NULL)
	{
		array[this->index] = this->content;
		this = this->next;
	}
	return (array);
}
