/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specular.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 15:39:00 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/11 14:23:23 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "minirt.h"

bool	specular_highlight(
			t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *result)
{
	if (hit_sphere(origin, obj, ray, result))
		return (true);
	return (false);
}
