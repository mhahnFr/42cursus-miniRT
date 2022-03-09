/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mixer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhahn <mhahn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:51:25 by mhahn             #+#    #+#             */
/*   Updated: 2022/03/09 19:51:27 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "minirt.h"

t_mixer	*init_mainstruct(int *err)
{
	t_mixer	*ret;

	ret = malloc(sizeof(struct s_mixer));
	if (ret == NULL)
	{
		*err = 3;
		return (NULL);
	}
	return (ret);
}
