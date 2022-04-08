/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 14:22:16 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/08 17:27:34 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*p;

	p = ft_gc_malloc(size * count);
	if (p == NULL)
		return (NULL);
	ft_bzero(p, count * size);
	return (((void *)p));
}
