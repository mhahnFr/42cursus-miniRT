#include <stdlib.h>

#include "mlx.h"

#include "renderer_image.h"

t_renderer_image	*renderer_image_new(
						void *mlx_ptr,
						size_t width,
						size_t height)
{
	t_renderer_image	*ret;

	if (mlx_ptr == NULL)
		return (NULL);
	ret = malloc(sizeof(struct s_renderer_image));
	if (ret == NULL)
		return (NULL);
	ret->mlx_img = mlx_new_image(mlx_ptr, width, height);
	ret->mlx_ptr = mlx_ptr;
	renderer_image_create(ret, ret->mlx_img, width, height);
	return (ret);
}

void	renderer_image_create(
			t_renderer_image *this,
			void *mlx_img,
			size_t width,
			size_t height)
{
	if (this == NULL || mlx_img == NULL)
		return ;
	this->mlx_img = mlx_img;
	this->width = width;
	this->height = height;
	this->background_color = 0;
	this->raw = mlx_get_data_addr(mlx_img, &(this->depth), &(this->line_size),
			&(this->endian));
}

void	renderer_image_destroy(t_renderer_image *this)
{
	if (this == NULL)
		return ;
	if (this->mlx_img != NULL)
		mlx_destroy_image(this->mlx_ptr, this->mlx_img);
	this->mlx_img = NULL;
}

void	renderer_image_delete(t_renderer_image *this)
{
	if (this == NULL)
		return ;
	renderer_image_destroy(this);
	free(this);
}
