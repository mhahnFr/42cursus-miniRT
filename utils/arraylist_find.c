#include "arraylist.h"

t_arraylist	*arraylist_find(t_arraylist *this, void *content)
{
	while (this != NULL)
	{
		if (this->content == content)
			return (this);
		this = this->next;
	}
	return (NULL);
}
