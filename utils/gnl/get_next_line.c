/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 20:47:52 by mhahn             #+#    #+#             */
/*   Updated: 2021/11/25 20:47:53 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "get_next_line_utils.h"
#include "minirt.h"

t_string_builder	*string_builder_new(void)
{
	t_string_builder	*this;

	this = ft_gc_malloc(sizeof(t_string_builder) + GNL_BUFFER * sizeof(char));
	if (this != NULL)
	{
		this->new_line = NULL;
		this->next = NULL;
		this->start_offset = 0;
		this->string_length = 0;
	}
	return (this);
}

size_t	string_builder_size_nl(t_string_builder *this)
{
	size_t	size;

	size = 0;
	while (this != NULL)
	{
		if (this->new_line == NULL)
			size += (this->string_length - this->start_offset);
		else
		{
			size += (this->new_line - this->part) - this->start_offset + 1;
			break ;
		}
		this = this->next;
	}
	return (size);
}

char	*string_builder_to_string_nl(t_string_builder *this)
{
	char	*str;
	size_t	mover;
	size_t	length;

	str = NULL;
	length = string_builder_size_nl(this);
	if (length == 0)
		return (NULL);
	str = ft_gc_malloc(length + 1);
	if (str == NULL)
		return (NULL);
	mover = 0;
	while (this != NULL)
	{
		if (this->new_line == NULL)
			length = (this->string_length - this->start_offset);
		else
			length = (this->new_line - this->part) - this->start_offset + 1;
		gnl_memcpy(str + mover, this->part + this->start_offset, length);
		mover += length;
		this = this->next;
	}
	*(str + mover) = '\0';
	return (str);
}

t_string_builder	*string_builder_cut_nl(t_string_builder *this)
{
	t_string_builder	*tmp;

	while (this != NULL && this->new_line == NULL)
	{
		tmp = this->next;
		ft_gc_free(this);
		this = tmp;
	}
	if (this != NULL)
	{
		this->start_offset = this->new_line - this->part + 1;
		this->new_line = ft_memchr(this->part + this->start_offset,
				'\n',
				this->string_length - this->start_offset);
	}
	return (this);
}

char	*get_next_line(int fd)
{
	static t_string_builder	*builder = NULL;
	char					*line;

	line = NULL;
	if (!string_builder_has_new_line(builder))
	{
		if (!read_line(&builder, fd))
			return (NULL);
	}
	line = string_builder_to_string_nl(builder);
	builder = string_builder_cut_nl(builder);
	return (line);
}
