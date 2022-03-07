#include "libft.h"

#include "hex_str_to_unsigned_int.h"

bool	ft_ishexnum(char c)
{
	return (ft_isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}

bool	ft_isupper(char c)
{
	return (c >= 'A' && c <= 'Z');
}

unsigned int	ft_get_value(char c)
{
	if (!ft_ishexnum(c))
		return (0);
	if (ft_isdigit(c))
		return (c - '0');
	if (ft_isupper(c))
		return (c - 'A' + 10);
	return (c - 'a' + 10);
}

unsigned int	hexstr_to_uint(char *string)
{
	size_t			i;
	unsigned int	num;

	i = 0;
	num = 0;
	while (string[i] != '\0' && ft_ishexnum(string[i]))
	{
		num = (num * 16) + (ft_get_value(string[i]));
		i++;
	}
	return (num);
}
