/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 18:06:16 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/08 17:49:08 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "mlx.h"
#include "libft.h"
#include <stdio.h>

void	vector_print(t_vector *vec)
{
	printf("%f | %f | %f\n", vec->x, vec->y, vec->z);
}

static void	print_lexer_error(int error)
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
	ft_gc_exit(error);
}

static void	print_error(int error)
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
		ft_putendl_fd("Unknown error occurred", 1);
	ft_gc_exit(error);
}

static void	open_mlx(t_mixer *all_struct)
{
	all_struct->p_mlx_window = mlx_new_window(
			all_struct->p_mlx_init, all_struct->res_x,
			all_struct->res_y, "miniRT");
	mlx_put_image_to_window(all_struct->p_mlx_init,
		all_struct->p_mlx_window, all_struct->image->mlx_img, 0, 0);
	mlx_key_hook(all_struct->p_mlx_window, key_handler, all_struct);
	mlx_hook(all_struct->p_mlx_window, 17, 0, key_redcross, all_struct);
	mlx_loop(all_struct->p_mlx_init);
}

int	main(int argc, char **argv)
{
	t_mixer	*all_struct;
	int		ret;

	ret = 0;
	all_struct = NULL;
	if (argc < 2)
		ret = 1;
	else
	{
		all_struct = init_mainstruct(&ret);
		config_mixer(all_struct, argv, argc);
		if (all_struct == NULL)
			print_error(3);
		init_mixer_image(all_struct);
		lexer(argv[1], all_struct, &ret);
		print_error(ret);
		rt_cam(all_struct);
		rt_start(all_struct);
		print_error(ret);
		open_mlx(all_struct);
	}
	print_error(ret);
	return (ret);
}
