
void	renderer_draw_point(t_point *point, t_renderer_image *buf)
{
	char	*dst;

	if (point->x < (long) buf->width && point->x >= 0
		&& point->y < (long) buf->height && point->y >= 0)
	{
		dst = buf->raw + (long) point->x * (buf->depth / 8) + (long) point->y
			* buf->line_size;
		*(unsigned int *) dst = (0 << 24) + (point->r << 16) + (point->g << 8)
			+ (point->b << 0);
	}
}
