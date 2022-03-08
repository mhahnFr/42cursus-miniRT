/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc_charpp.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 15:39:06 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/08 15:40:34 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*freetili(char **arr)
{
	int	i;

	i = 0;
	while (arr != NULL && arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	if (arr != NULL)
		free (arr);
	return (NULL);
}

char	**ft_realloc_charpp(char **old, size_t new_size)
{
	char	**new;
	int		i;
	int		ii;

	new = ft_calloc(new_size, sizeof(char *));
	if (new == NULL)
		return (NULL);
	i = 0;
	while (old != NULL && old[i] != NULL)
	{
		ii = 0;
		new[i] = ft_calloc(ft_strlen(old[i]) + 1, 1);
		if (new[i] == NULL)
			return (freetili(new));
		while (old[i][ii] != '\0')
		{
			new[i][ii] = old[i][ii];
			ii++;
		}
		new[i][ii] = '\0';
		i++;
	}
	if (old != NULL)
		freetiliold(old);
	return (new);
}