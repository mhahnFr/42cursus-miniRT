/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 18:06:16 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/09 19:45:44 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "mlx.h"
#include "libft.h"

void	printerror(int error) {
	if (!error)
		return ;
	ft_putendl_fd("Error", 1);
	if (error == 1)
		ft_putendl_fd("Arguments", 1);
	else if (error == 2)
		ft_putendl_fd("File", 1);
	else if (error == 3)
		ft_putendl_fd("Memory", 1);
	else
		ft_putendl_fd("Unknown error occured", 1);
}

int main(int argc, char **argv)
{
	t_mixer	*all_struct;
	int		ret;

	ret = 0;
	if (argc != 2)
		ret = 1;
	else {
		all_struct = init_mainstruct(&ret);
		if (all_struct != NULL)
			lexer(argv[1], all_struct, &ret);
		if (all_struct != NULL && ret == 0)
			rt_cam_init(all_struct);
		//if (all_struct != NULL)
		//	calculator(all_struct, &ret);
		//if (all_struct != NULL)
		//	paint(all_struct, &ret);
	}
	printerror(ret);
	ft_gc_clear();
	return ret;
}
