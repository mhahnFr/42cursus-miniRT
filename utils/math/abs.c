/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:37:29 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/07 16:37:30 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

#include "abs.h"

long	labs(long value)
{
	if (value == LONG_MIN)
		return (value);
	if (value < 0)
		return (value * -1);
	return (value);
}

int	abs(int value)
{
	if (value == INT_MIN)
		return (value);
	if (value < 0)
		return (value * -1);
	return (value);
}
