/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 20:47:50 by mhahn             #+#    #+#             */
/*   Updated: 2021/11/25 20:47:51 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_UTILS_H
# define GET_NEXT_LINE_UTILS_H

# include <stdlib.h>
# include <stdbool.h>

/*
 * A string builder part. It includes the actual string part of the size of
 * the specified buffer size plus one for the null terminator. There is also
 * the pointer to the newline character in the string part, if a newline
 * character is present. The offset indicates at which position to start. The
 * length of the string can also be stored. Of course, there is also a pointer
 * to the next part.
 */
typedef struct s_string_builder {
	char					*new_line;
	size_t					start_offset;
	size_t					string_length;
	struct s_string_builder	*next;
	char					part[];
}	t_string_builder;

/*
 * Copies the given amount of bytes from the given source to
 * the given destination. It does not handle possible memory
 * overlays. Use ft_memmove if an overlay of the memory could
 * be possible. Returns the given destination.
 */
void				*gnl_memcpy(void *dst, const void *src, size_t n);

/*
 * Reads one line from the file determined by the given file descriptor into
 * the given string builder. Depending on the specified buffer size, some
 * characters could follow after the newline character.
 */
bool				read_line(t_string_builder **builder, int fd);

/*
 * Allocates a new string builder part. All values are set to null. Returns the
 * newly allocated, empty string builder part.
 */
t_string_builder	*string_builder_new(void);

/*
 * Calculates the string length of the string builder. Counting stops at the
 * first newline character encountered. It does not search for newline
 * characters in the strings, it only checks for the newline variable in the
 * struct. If no newline is encountered, simply returns the length until the
 * null terminator in the last string builder part. Returns the calculated
 * size.
 */
size_t				string_builder_size_nl(t_string_builder *this);

/*
 * Creates a string consisting of all string builder parts until the newline
 * character or the end is reached. Returns a newly allocated and null
 * terminated string.
 */
char				*string_builder_to_string_nl(t_string_builder *this);

/*
 * Deletes the leading string builder parts before the part with the newline
 * character set. Sets then the offset to the position of the newline
 * character. Returns the cutted string builder.
 */
t_string_builder	*string_builder_cut_nl(t_string_builder *this);

/*
 * Checks each node if the newline character is set. Stops checking at the
 * first encountered newline. Returns true, if a newline is found within the
 * given string builder, false otherwise or if null is given.
 */
bool				string_builder_has_new_line(t_string_builder *this);

/*
 * Appends the given string builder part to the given string builder. If the
 * string builder is null, the given appendix takes the first place in the
 * string builder. The function also sets the length information of the
 * given appendix using the given length. Also, it adds the null terminator to
 * the string of the given appendix.
 */
void				string_builder_append(
						t_string_builder **this,
						t_string_builder *appendix,
						size_t length);
#endif
