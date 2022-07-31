/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 20:47:47 by mhahn             #+#    #+#             */
/*   Updated: 2021/11/25 20:47:48 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "get_next_line_utils.h"
#include "get_next_line.h"
#include "libft.h"

void	*gnl_memcpy(void *dst, const void *src, size_t n)
{
	size_t	counter;

	counter = 0;
	if ((src != NULL || dst != NULL) && n > 0)
	{
		while (counter < n)
		{
			((char *) dst)[counter] = ((char *) src)[counter];
			counter++;
		}
	}
	return (dst);
}

bool	read_line(t_string_builder **builder, int fd)
{
	t_string_builder	*tmp;
	char				*newline;
	size_t				ret;

	newline = NULL;
	while (newline == NULL)
	{
		tmp = string_builder_new();
		if (tmp == NULL)
			return (false);
		ret = read(fd, tmp->part, GNL_BUFFER);
		if (ret <= 0)
		{
			ft_gc_free(tmp);
			break ;
		}
		tmp->new_line = ft_memchr(tmp->part, '\n', ret);
		newline = tmp->new_line;
		string_builder_append(builder, tmp, ret);
	}
	return (true);
}

void	string_builder_append(t_string_builder **this,
							t_string_builder *appendix,
							size_t length)
{
	t_string_builder	*tmp;

	appendix->string_length = length;
	if (*this == NULL)
	{
		*this = appendix;
		return ;
	}
	tmp = *this;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = appendix;
}

bool	string_builder_has_new_line(t_string_builder *this)
{
	while (this != NULL)
	{
		if (this->new_line != NULL)
			return (true);
		this = this->next;
	}
	return (false);
}
