#ifndef RENDERER_IMAGE_H
# define RENDERER_IMAGE_H

# include <stddef.h>

/*
 * Represents an image. Contains all informations of the MiniLibX image.
 * Contains the background color as well.
 */
typedef struct s_renderer_image
{
	int				depth;
	int				line_size;
	int				endian;
	size_t			width;
	size_t			height;
	unsigned int	background_color;
	char			*raw;
	void			*mlx_img;
	void			*mlx_ptr;
}	t_renderer_image;

/*
 * Allocates a new image using the given MiniLibX pointer, its width and
 * height. Returns either the newly allocated image or null, if no MiniLibX
 * pointer is given or the allocation failed.
 */
t_renderer_image	*renderer_image_new(
						void *mlx_ptr,
						size_t width,
						size_t height);

/*
 * Initializes the given image using the given image pointer and its width and
 * height. Does nothing if no image or no MiniLibX image pointer is given.
 */
void				renderer_image_create(
						t_renderer_image *this,
						void *mlx_img,
						size_t width,
						size_t height);

/*
 * Destroys the given image. Also destroys the corresponding MiniLibX image.
 * Does nothing if no image is given.
 */
void				renderer_image_destroy(t_renderer_image *this);

/*
 * Deletes the given image. Also destroys the corresponding MiniLibX image.
 * Does nothing if no image is given.
 */
void				renderer_image_delete(t_renderer_image *this);

#endif
