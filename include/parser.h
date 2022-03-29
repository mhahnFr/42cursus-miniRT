/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 15:59:28 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/29 15:24:10 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "libft.h"
# include "minirt.h"

t_rgbof		get_color(char *src);
t_vector	get_vector(char *src);
int			add_object(char *buffer, t_mixer *m_data);
int			add_cylinder(char **line, t_mixer *m_data);
int			add_sphere(char **line, t_mixer *m_data);
int			add_plane(char **line, t_mixer *m_data);
#endif /* PARSER_H */
