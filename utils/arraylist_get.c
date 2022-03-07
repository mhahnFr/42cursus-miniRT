/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arraylist_get.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:36:50 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/07 16:36:51 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arraylist.h"

void	*arraylist_get(t_arraylist *this, size_t index)
{
	return (arraylist_get_element(this, index)->content);
}

void	*arraylist_get_unsafe(t_arraylist *this, size_t index)
{
	return (arraylist_get_element_unsafe(this, index)->content);
}

t_arraylist	*arraylist_get_element(t_arraylist *this, size_t index)
{
	size_t		counter;

	counter = 0;
	while (this != NULL)
	{
		if (counter == index)
			return (this);
		counter++;
		this = this->next;
	}
	return (NULL);
}

t_arraylist	*arraylist_get_element_unsafe(t_arraylist *this, size_t index)
{
	while (this != NULL)
	{
		if (this->index == index)
			return (this);
		this = this->next;
	}
	return (NULL);
}
