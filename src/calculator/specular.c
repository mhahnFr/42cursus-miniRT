/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specular.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 15:39:00 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/03 17:04:52 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <math.h>
#include "minirt.h"
#include "libft.h"

bool	specular_highlight(t_vector *origin, t_obj_l *obj, t_vector *ray, t_vector *result)
{
	if (hit_sphere(origin, obj, ray, result))
		return (true);
	return (false);
}
