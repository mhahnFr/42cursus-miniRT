/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hex_str_to_unsigned_int.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:38:29 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/07 16:38:30 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEX_STR_TO_UNSIGNED_INT_H
# define HEX_STR_TO_UNSIGNED_INT_H

# include <stdbool.h>

/*
 * Returns wether the given char is a hexadecimal digit.
 */
bool			ft_ishexnum(char c);

/*
 * Returns wether the given character is in uppercase.
 */
bool			ft_isupper(char c);

/*
 * Returns the value of the given hexadecimal digit. Returns zero if the given
 * character is no hexadecimal number.
 */
unsigned int	ft_get_value(char c);

/*
 * Parses a given string and converts it to an unsigned int, assuming given
 * string contains a hexadecimal number. The number must not be preceded by
 * any character, but can be followed by any amount of characters. Returns zero
 * if an error occurred.
 */
unsigned int	hexstr_to_uint(char *string);

#endif
