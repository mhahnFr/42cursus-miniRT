#include <limits.h>

#include "abs.h"

long	labs(long value)
{
	if (value == LONG_MIN)
		return (value);
	if (value < 0)
		return (value * -1);
	return (value);
}

int	abs(int value)
{
	if (value == INT_MIN)
		return (value);
	if (value < 0)
		return (value * -1);
	return (value);
}
