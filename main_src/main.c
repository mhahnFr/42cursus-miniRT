/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 18:06:16 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/07 18:41:33 by jkasper          ###   ########.fr       */
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
		ft_putchar("Memory", 1);
	else
		ft_putchar("Unknown error occured", 1);
}

int main(int argc, char **argv) {
	int	ret;
	void	*all_struct;

	ret = 0;
	if (argc != 2)
		ret = 1;
	else {
		all_struct = init_mainstruct(char argv[1], &ret);
		if (all_struct)
			lexer(all_struct, &ret);
		//if (all_struct)
		//	builder(all_struct, &ret);//maybe needed
		if (all_struct)
			calculator(all_struct, &ret);
		if (all_struct)
			paint(all_struct, &ret);
	}
	printerror(ret);
	return ret;
}
