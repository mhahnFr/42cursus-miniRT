#include <stdlib.h>

#include "arraylist.h"

t_arraylist	*arraylist_new(void *content)
{
	t_arraylist	*ret;

	ret = malloc(sizeof(struct s_arraylist));
	if (ret != NULL)
	{
		ret->content = content;
		ret->previous = NULL;
		ret->next = NULL;
		ret->index = 0;
	}
	return (ret);
}

bool	arraylist_append_unsafe(t_arraylist **this, t_arraylist *appendix)
{
	if (this == NULL)
		return (false);
	else if (*this == NULL)
	{
		*this = appendix;
		(*this)->index = 0;
		(*this)->previous = *this;
		return (true);
	}
	(*this)->previous->next = appendix;
	appendix->previous = (*this)->previous;
	(*this)->previous = appendix;
	appendix->index = appendix->previous->index + 1;
	return (true);
}

bool	arraylist_append(t_arraylist **this, t_arraylist *appendix)
{
	t_arraylist	*tmp;
	size_t		counter;

	if (this == NULL)
		return (false);
	else if (*this == NULL)
	{
		*this = appendix;
		(*this)->index = 0;
		return (true);
	}
	tmp = *this;
	counter = 1;
	while (tmp->next != NULL)
	{
		tmp = tmp->next;
		counter += 1;
	}
	tmp->next = appendix;
	tmp->next->previous = tmp;
	appendix->index = counter;
	return (true);
}

bool	arraylist_append_unsafe_old(t_arraylist **this, t_arraylist *appendix)
{
	t_arraylist	*tmp;

	if (this == NULL)
		return (false);
	else if (*this == NULL)
	{
		*this = appendix;
		(*this)->index = 0;
		return (true);
	}
	tmp = *this;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = appendix;
	tmp->next->previous = tmp;
	appendix->index = tmp->index + 1;
	return (true);
}
