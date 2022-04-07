/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 18:06:16 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/07 11:45:22 by mhahn            ###   ########.fr       */
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
	else if (error == 25)
		ft_putendl_fd("At least one required object missing", 1);
	else
		ft_putendl_fd("Error while parsing", 1);
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
	all_struct = NULL;
	if (argc != 2)
		ret = 1;
	else
	{
		all_struct = init_mainstruct(&ret);
		if (all_struct != NULL)
			lexer(argv[1], all_struct, &ret);
		if (all_struct != NULL && ret == 0)
		{
			rt_cam(all_struct);
			rt_start(all_struct);
		}
		if (ret != 0)
		{
			printerror(ret);
			exit(1);
		}
		all_struct->p_mlx_window = mlx_new_window(all_struct->p_mlx_init, RESOLUTION_X, RESOLUTION_Y, "TRASH!");
		mlx_put_image_to_window(all_struct->p_mlx_init, all_struct->p_mlx_window, all_struct->image->mlx_img, 0, 0);
		mlx_key_hook(all_struct->p_mlx_window, key_handler, all_struct);
		mlx_hook(all_struct->p_mlx_window, 17, 0, key_redcross, all_struct);
		mlx_loop(all_struct->p_mlx_init);
	}
	printerror(ret);
	exit(ret);
	return (ret);
}


/*

lexer			main
camera			main

threadbuiltup ||
calculus      ||
print			main

*/
