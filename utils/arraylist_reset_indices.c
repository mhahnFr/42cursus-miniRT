/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arraylist_reset_indices.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:36:58 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/07 16:36:59 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arraylist.h"

void	arraylist_reset_indices(t_arraylist *this)
{
	arraylist_reset_indices_start(this, 0);
}

void	arraylist_reset_indices_start(t_arraylist *this, size_t start)
{
	while (this != NULL)
	{
		this->index = start++;
		this = this->next;
	}
}
