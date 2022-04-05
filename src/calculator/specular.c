/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specular.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 15:39:00 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/05 18:31:11 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <math.h>
#include "minirt.h"
#include "libft.h"

bool	specular_highlight(t_thread *self, t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *result)
{
	return (hit_sphere(self, origin, obj, ray, result));
}
