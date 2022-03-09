/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 15:09:31 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/09 19:43:16 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "get_next_line.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int	checkfile(char *name)
{
	int	i;

	i = ft_strlen(name);
	if (i < 4 || name[i - 1] != 't' || name[i - 2] != 'r' || name[i - 3] != '.')
		return (1);
	return (0);
}

char	**loader(char *path, int *errnum, int *size)
{
	int		i;
	int		buff_time;
	int		fd;
	char	**buffer;

	if (checkfile(path))
	{
		*errnum = 2;
		return (NULL);
	}
	fd = open(path, O_RDONLY);
	if (fd < 0)
		*errnum = 2;
	buffer = ft_calloc(LEXER_BUFFER + 1, sizeof(char *));
	if (buffer == NULL)
	{
		close(fd);
		*errnum = 3;
		return NULL;
	}
	i = 0;
	buff_time = 1;
	buffer[i] = get_next_line(fd);
	while (buffer[i] != NULL)
	{
		i++;
		if (i == LEXER_BUFFER * buff_time) 
		{
			buffer = ft_realloc_charpp(buffer, (LEXER_BUFFER * ++buff_time) + 1);
			if (buffer == NULL)
			{
				*errnum = 3;
				break ;
			}
		}
		buffer[i] = get_next_line(fd);
	}
	*size = i;
	close(fd);
	return buffer;
}

//cant handle trailing whitespaces before nl
int	lexer(char *path, t_mixer *m_data, int *errnum)
{
	char	**buffer;
	int		size;

	size = 0;
	(void) m_data;
	buffer = loader(path, errnum, &size);
	if (buffer == NULL || size < 3)
		return(1);
	if (validation_check(buffer, size))
	{
		ft_free_char_arr(buffer);
		*errnum = 2;
		return(1);
	}
	if (parser(buffer, m_data, size))
	{
		ft_free_char_arr(buffer);
		*errnum = 3;
		return(1);
	}
	ft_free_char_arr(buffer);
	printf("file read successfull!\n");
	return (0);
}
