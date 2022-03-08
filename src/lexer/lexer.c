/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 15:09:31 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/08 16:39:52 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "get_next_line.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>

char	**loader(char *path, int *retval, int *size)
{
	int		i;
	int		buff_time;
	int		fd;
	char	**buffer;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		*retval = 2;
	buffer = ft_calloc(LEXER_BUFFER + 1, sizeof(char *));
	if (buffer == NULL)
	{
		close(fd);
		*retval = 3;
		return NULL;
	}
	i = 0;
	buff_time = 1;
	while (buffer[i] == NULL)
	{
		if (i == LEXER_BUFFER * buff_time) 
		{
			buffer = ft_realloc_charpp(buffer, (LEXER_BUFFER * ++buff_time) + 1);
			if (buffer == NULL)
			{
				*retval = 3;
				break ;
			}
		}
		buffer[i] = get_next_line(fd);
		i++;
	}
	*size = i;
	close(fd);
	return buffer;
}

int	lexer(char *path, t_mixer *m_data, int *retval)
{
	char	**buffer;
	int		size;

	size = 0;
	buffer = loader(path, retval, &size);
	if (buffer == NULL)
		return(1);
	if (validation_check(buffer, size))
	{
		//free buffer
		*retval = 2;
		return(1);
	}
	if (parser(buffer, m_data, size))
	{
		//free buffer and maybe m_data?
		*retval = 3;
		return(1);
	}
}
