/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 18:06:16 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/10 16:54:23 by mhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "mlx.h"
#include "libft.h"

void	print_lexer_error(int error)
{
	if (error == 21)
		ft_putendl_fd("File loading failed", 1);
	else if (error == 22)
		ft_putendl_fd("File validation failed", 1);
	else if (error == 23)
		ft_putendl_fd("Memory allocation failed", 1);
	else if (error == 24)
		ft_putendl_fd("File descriptor error", 1);
	else
		ft_putendl_fd("Unknown error while parsing", 1);
}

void	printerror(int error)
{
	if (!error)
		return ;
	ft_putendl_fd("Error", 1);
	if (error >= 30)
		ft_putendl_fd("execution failed", 1);
	else if (error >= 20)
		print_lexer_error(error);
	else if (error == 1)
		ft_putendl_fd("bad Arguments", 1);
	else if (error == 3)
		ft_putendl_fd("Memory allocation failed", 1);
	else
		ft_putendl_fd("Unknown error occured", 1);
}

int	main(int argc, char **argv)
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
		if (all_struct != NULL && ret == 0)
			calculator(all_struct, &ret);
		//if (all_struct != NULL)
		//	paint(all_struct, &ret);
	}
	printerror(ret);
	ft_gc_clear();
	return (ret);
}
