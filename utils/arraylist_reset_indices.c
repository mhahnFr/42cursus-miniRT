#include "arraylist.h"

void	arraylist_reset_indices(t_arraylist *this)
{
	arraylist_reset_indices_start(this, 0);
}

void	arraylist_reset_indices_start(t_arraylist *this, size_t start)
{
	while (this != NULL)
	{
		this->index = start++;
		this = this->next;
	}
}
