/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 15:09:31 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/13 14:08:48 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "get_next_line.h"
#include "libft.h"
#include "lexer.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int	checkfile(char *name)
{
	size_t	i;

	i = ft_strlen(name);
	if (i < 4 || name[i - 1] != 't' || name[i - 2] != 'r' || name[i - 3] != '.')
		return (1);
	return (0);
}

int	read_to_buffer(int fd, char ***alloc, int *size)
{
	int		i;
	int		buff_time;
	char	**buff;

	i = 0;
	buff_time = 1;
	buff = *alloc;
	buff[i] = get_next_line(fd);
	while (buff[i] != NULL)
	{
		i++;
		if (i == LEXER_BUFFER * buff_time)
		{
			buff = ft_realloc_charpp(buff, (LEXER_BUFFER * ++buff_time) + 1);
			if (buff == NULL)
				return (3);
		}
		buff[i] = get_next_line(fd);
	}
	*size = i;
	return (0);
}

char	**loader(char *path, int *errnum, int *size)
{
	int		fd;
	char	**buff;

	if (checkfile(path))
	{
		*errnum = 1;
		return (NULL);
	}
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		*errnum = 4;
		return (NULL);
	}
	buff = ft_calloc(LEXER_BUFFER + 1, sizeof(char *));
	if (buff == NULL)
	{
		close(fd);
		*errnum = 3;
		return (NULL);
	}
	*errnum = read_to_buffer(fd, &buff, size);
	close(fd);
	return (buff);
}

//can't handle trailing whitespaces before nl
int	lexer(char *path, t_mixer *m_data, int *errnum)
{
	char	**buffer;
	int		size;

	size = 0;
	buffer = loader(path, errnum, &size);
	if (buffer == NULL || size < 3 || *errnum)
	{
		*errnum += 20;
		return (*errnum);
	}
	if (validation_check(buffer, size))
	{
		*errnum = 22;
		return (*errnum);
	}
	*errnum = parser(buffer, m_data, size);
	if (*errnum != 0)
	{
		*errnum += 20;
		return (1);
	}
	correct_normals(m_data);
	printf("file read successfully!\n");
	return (0);
}
