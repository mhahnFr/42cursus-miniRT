#ifndef ARRAYLIST_H
# define ARRAYLIST_H

# include <stddef.h>
# include <stdbool.h>

/*
 * Represents an element of a linked list, which consists of an array of any
 * type, and a pointer to the next element. The index is automatically
 * overwritten by the appending method.
 */
typedef struct s_arraylist {
	void				*content;
	size_t				index;
	struct s_arraylist	*previous;
	struct s_arraylist	*next;
}	t_arraylist;

/*
 * Represents a function that can duplicate the specific content of an
 * arraylist. It takes the content of an element a parameter and returns newly
 * allocated copy of it.
 */
typedef void	*(*t_arraylist_dup)(void *);

/*
 * Represents a function that can delete the specific content of an arraylist.
 * It takes the content of an element as parameter and removes it. It does not
 * return anything.
 */
typedef void	(*t_arraylist_remover)(void *);

/*
 * Returns a newly allocated arraylist, containing the given content.
 */
t_arraylist		*arraylist_new(void *content);

/*
 * Returns the content of the element at the given index in the given
 * arraylist. Returns null if no list is given or the given index does not
 * exist.
 */
void			*arraylist_get(t_arraylist *this, size_t index);

/*
 * Returns the element at the given index in the given arraylist. Returns null
 * if no list is given or the given index does not exist in the given list.
 */
t_arraylist		*arraylist_get_element(
					t_arraylist *this,
					size_t index);

/*
 * Returns the content of the element at the given index in the given
 * arraylist. Returns null if no list is given or the given index does not
 * exist. As this unsafe version is relying on the index of each element, which
 * can be manipulated, the content of the first element with the given index is
 * returned, which is not necessarily equal to the count of elements in the
 * list.
 */
void			*arraylist_get_unsafe(t_arraylist *this, size_t index);

/*
 * Returns the element at the given index in the given arraylist. Returns null
 * if no list is given or the given index does not exist in the given list. As
 * this unsafe version is relying on the index of each element, which can be
 * manipulated, the first element with the given index is returned, which is
 * not necessarily equal to the count of elements in the list.
 */
t_arraylist		*arraylist_get_element_unsafe(
					t_arraylist *this,
					size_t index);

/*
 * Inserts the given element at the given index in the given arraylist. The
 * given element will have the given index. If the index does not exist in the
 * given arraylist, the element is appended at the end of the list, and the
 * element will have the coresponding index. Returns true if the element was
 * inserted as desired, false if the element was appended at the end of the
 * list or if at least one parameter is missing or the given index is outside
 * of the given list.
 */
bool			arraylist_insert_at(
					t_arraylist **this,
					t_arraylist *element,
					size_t index);

/*
 * Inserts the given element before the given element in the given arraylist.
 * If the given element does not exist in the given arraylist, the new element
 * will be appended at the end of the given arraylist. Returns true, if the
 * element was appended as desired, false if the element was appended at the
 * end of the list or if at least one parameter is missing.
 */
bool			arraylist_insert_before(
					t_arraylist **this,
					t_arraylist *element,
					t_arraylist *new);

/*
 * Inserts the given element after the given element in the given arraylist.
 * If the given element does not exist in the given arraylist, the new element
 * will be appended at the end of the given arraylist. Returns true, if the
 * element was appended as desired, false if the element was appended at the
 * end of the list or if at least one parameter is missing.
 */
bool			arraylist_insert_after(
					t_arraylist **this,
					t_arraylist *element,
					t_arraylist *new);
/*
 * Deletes the element at the given index out of the list and deletes that
 * element afterwards. The content of that element will be deleted by the
 * optionally given delete function. Returns true, if the element at the given
 * index was successfully deleted, false if no list is given or the given index
 * does not exist in the given list. Does not delete anything if the index is
 * outside of the list.
 */
bool			arraylist_remove_at_index(
					t_arraylist **this,
					size_t index,
					t_arraylist_remover remover);

/*
 * Deletes the given element out of the list, deletes the element afterwards.
 * The pointer to the given element will be set to null. The content of that
 * element will be deleted with the given delete function. This function is
 * optional. Returns true if the given element was successfully deleted, false
 * if no list or no element is given.
 */
bool			arraylist_remove_at_element(
					t_arraylist **this,
					t_arraylist **element,
					t_arraylist_remover remover);

/*
 * Adds the given element to the given arraylist. If the pointer to the
 * beginning of the list is null, the appendix is the new beginning. If the
 * pointer to the beginning itself is null, this function does nothing. The
 * index of the appended element is also set. Returns true if the given element
 * was successfully appended to the given list, false if no list is given.
 */
bool			arraylist_append(t_arraylist **this, t_arraylist *appendix);

/*
 * Adds the given element to the given arraylist. If the pointer to the
 * beginning of the list is null, the appendix is the new beginning. If the
 * pointer to the beginning itself is null, this function does nothing. The
 * index of the appended element is also set, using the index of the previous
 * element, if there is one. Thereby, as this unsafe version is using the
 * changable index of each element, the index of the appended element might be
 * wrong in case the index of the last element has been manipulated. It does
 * not crash because of this. Returns true if the given element was
 * successfully appended to the given list, false if no list is given.
 */
bool			arraylist_append_unsafe(
					t_arraylist **this,
					t_arraylist *appendix);

/*
 * Converts the given arraylist to a simple array. Note that any changes made
 * to either object are not tracked by the other one, as the array contains
 * duplicates of the contents of the arraylist using the given function. If an
 * empty arraylist is given, an empty array is returned. Returns either the
 * newly allocated array or null if either the allocation failed or no
 * arraylist or no duplicate function is given.
 */
void			**arraylist_to_array(
					t_arraylist *this,
					t_arraylist_dup dup);

/*
 * Converts the given arraylist to a simple array. Please note that any changes
 * made to either object are not tracked by the other one as the array contains
 * duplicates of the contents of the arraylist. If an empty arraylist is given,
 * an empty array is returned. Returns either the newly alloacted array or null
 * if either the allocation failed or no arraylist or no duplicate function is
 * given. As this unsafe version is using the changable index of each element,
 * this method might crash in case the indices of the elements in the given
 * arraylist are manipulated.
 */
void			**arraylist_to_array_unsafe(
					t_arraylist *this,
					t_arraylist_dup dup);

/*
 * Converts the given arraylist to a simple array. The contents of the given
 * arraylist are not duplicated, the given arraylist is deleted after the
 * transformation into an array without deleting the contents. If an empty
 * arraylist is given, an empty array is returned. Returns either the newly
 * allocated array or null if the allocation failed. If the array could not be
 * created, the given arraylist will not be deleted.
 */
void			**arraylist_to_array_transfer(t_arraylist **this);

/*
 * Converts the given arraylist to a simple array. The contents of the given
 * arraylist are not duplicated, but the given arraylist is also not deleted
 * after the transformation into an array. If an empty arraylist is given, an
 * empty array is returned. Returns either the newly allocated array or null if
 * the allocation failed.
 */
void			**arraylist_to_array_transfer_core(t_arraylist *this);

/*
 * Converts the given arraylist to a simple array. The contents of the given
 * arraylist are not duplicated, the given arraylist is deleted after the
 * transformation into an array without deleting the contents. If an empty
 * arraylist is given, an empty array is returned. Returns either the newly
 * allocated array or null if the allocation failed. If the array could not be
 * created, the given arraylist will not be deleted. As this method is using
 * other unsafe methods, it can crash or produce undefined behaviour and
 * results if the indices of the elements in the arraylist are manipulated.
 */
void			**arraylist_to_array_transfer_unsafe(
					t_arraylist **this);

/*
 * Converts the given arraylist to a simple array. The contents of the given
 * arraylist are not duplicated, but the given arraylist is also not deleted
 * after the transformation into an array. If an empty arraylist is given, an
 * empty array is returned. Returns either the newly allocated array or null if
 * the allocation failed. As this method is using other unsafe methods and
 * relies itself on the changable index of each element in the given arraylist,
 * it can crash or produce undefined behaviour and results if the indices of
 * the elements in the arraylist are manipulated.
 */
void			**arraylist_to_array_transfer_core_unsafe(
					t_arraylist *this);

/*
 * Deletes the given part of the arraylist. The function is an optional
 * function to delete the content of an element, if desired. Returns true if
 * the given element was deleted successfully, false if no element is given.
 */
bool			arraylist_delete(
					t_arraylist **this,
					t_arraylist_remover remover);

/*
 * Returns the count of elements in the given arraylist. If no arraylist is
 * given, zero is returned.
 */
size_t			arraylist_size(t_arraylist *this);

/*
 * Returns the count of elements in the given arraylist. If no arraylist is
 * given, zero is returned. As this unsafe version is using the changable index
 * of each element, this method might crash in case the indices of the elements
 * in the given arraylist are manipulated.
 */
size_t			arraylist_size_unsafe(t_arraylist *this);

/*
 * Deletes the whole given list. Uses the optionally given delete function to
 * remove the contents of each element. Returns true if the given list was
 * cleared successfully, false if no list is given.
 */
bool			arraylist_clear(
					t_arraylist **this,
					t_arraylist_remover remover);

/*
 * Resets the indices for the given arraylist. The index of the first element
 * will be zero, the second has the index one, and so on. Does nothing if no
 * list is given.
 */
void			arraylist_reset_indices(t_arraylist *this);

/*
 * Resets the indices for the given arraylist. The index of the first element
 * will be the given one, it is then incremented for every element. Does
 * nothing if no list is given.
 */
void			arraylist_reset_indices_start(t_arraylist *this, size_t start);

#endif
