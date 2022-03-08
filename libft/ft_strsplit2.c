/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 17:00:39 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/08 17:04:57 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_skip_chars(char *s, char *c, int *count)
{
	int	i;
	int	ii;

	i = *count;
	while (s[i] != '\0')
	{
		ii = 0;
		while (c[ii] != '\0')
		{
			if (s[i] == c[ii])
				break ;
			ii++;
		}
		if (c[ii] != '\0')
			break ;
		i++;
	}
	*count = i; 
}
