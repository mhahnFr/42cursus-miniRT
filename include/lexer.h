/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 15:47:21 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/10 15:53:17 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "libft.h"

int	check_sphere(char **splitline);
int	check_plane(char **splitline);
int	check_cylinder(char **splitline);
int	check_float(char *str);
int	check_vec_float(char *str, int *count);
int	check_rgb_int(char *str, int *count);
int	check_cam_int(char *str);
int	check_camera(char **splitline);
int	check_ambient(char **splitline);
int	check_light(char **splitline);
int	validation_check(char **buffer, int size);
int	check_vector(char *str);
int	check_orientation(char *str);
int	check_rgb(char *str);
#endif /* LEXER_H */
