/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arraylist_find.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:52:26 by mhahn             #+#    #+#             */
/*   Updated: 2022/03/09 19:52:29 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arraylist.h"

t_arraylist	*arraylist_find(t_arraylist *this, void *content)
{
	while (this != NULL)
	{
		if (this->content == content)
			return (this);
		this = this->next;
	}
	return (NULL);
}
