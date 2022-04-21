/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arraylist_remove.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:36:55 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/13 14:56:14 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "arraylist.h"

bool	arraylist_remove_at_element(
			t_arraylist **this,
			t_arraylist **element,
			t_arraylist_remover remover)
{
	t_arraylist	*tmp;

	if (this == NULL || *this == NULL || element == NULL)
		return (false);
	tmp = *this;
	while (tmp != NULL)
	{
		if (tmp == *element)
		{
			if (tmp->next != NULL)
				tmp->next->previous = tmp->previous;
			if (tmp->previous != NULL && tmp != *this)
				tmp->previous->next = tmp->next;
			if (tmp == (*this)->previous)
				(*this)->previous = tmp->previous;
			if (tmp == *this)
				*this = tmp->next;
			arraylist_delete(&tmp, remover);
			arraylist_reset_indices(*this);
			return (true);
		}
		tmp = tmp->next;
	}
	return (false);
}

bool	arraylist_remove_at_index(
			t_arraylist **this,
			size_t index,
			t_arraylist_remover remover)
{
	t_arraylist	*tmp;

	if (this == NULL)
		return (false);
	tmp = *this;
	while (tmp != NULL)
	{
		if (tmp->index == index)
		{
			if (tmp->next != NULL)
				tmp->next->previous = tmp->previous;
			if (tmp->previous != NULL)
				tmp->previous->next = tmp->next;
			if (tmp == *this)
				*this = tmp->next;
			arraylist_delete(&tmp, remover);
			arraylist_reset_indices(*this);
			return (true);
		}
		tmp = tmp->next;
	}
	return (false);
}

bool	arraylist_delete(t_arraylist **this, t_arraylist_remover remover)
{
	if (this == NULL || *this == NULL)
		return (false);
	if (remover != NULL)
		remover((*this)->content);
	free(*this);
	*this = NULL;
	return (true);
}

bool	arraylist_clear(t_arraylist **this, t_arraylist_remover remover)
{
	t_arraylist	*tmp;
	t_arraylist	*n;

	if (this == NULL)
		return (false);
	tmp = *this;
	while (tmp != NULL)
	{
		n = tmp->next;
		arraylist_delete(&tmp, remover);
		tmp = n;
	}
	*this = NULL;
	return (true);
}
