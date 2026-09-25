/* llist.c
 *  Arpan Bansal
 *  Abansal
 * ECE 2230 Fall 2026
 * MP2
 *
 * Purpose: A template for llist.c. You will make many changes.
 *
 * Assumptions: Many details are incomplete.
 *
 * Bugs: Many details have not been implemented.
 *
 */
#include <stdlib.h>
#include <assert.h>

#include "llist.h" // defines public functions for list ADT

// definitions for private constants used in llist.c only

#define LLIST_SORTED 989898
#define LLIST_UNSORTED -898989

// prototypes for private functions used in llist.c only

void llist_debug_validate(llist_t *L);

/* ----- below are the functions  ----- */

/* Obtains a pointer to an element stored in the specified list, at the
 * specified list position
 *
 * list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *           obtained from llist_construct.
 *
 * pos_index: position of the element to be accessed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LLPOSITION_FRONT and LLPOSITION_BACK
 *
 * return value: pointer to element accessed within the specified list.  A
 * value NULL is returned if the pos_index does not correspond to an element in
 * the list.
 */
data_t *llist_access(llist_t *list_ptr, int pos_index)
{
    llist_elem_t *rover = NULL;

    // debugging function to verify that the structure of the list is valid
    llist_debug_validate(list_ptr);

    /* handle special cases.
     *   1.  The list is empty
     *   2.  Asking for the head
     *   3.  Asking for the tail
     *   4.  specifying a position that is out of range.  This is not defined
     *       to be an error in this function, but instead it is assumed the
     *       calling function correctly specifies the position index
     */
    if (list_ptr->ll_entry_count == 0)
    {
        return NULL; // list is empty
    }
    else if (pos_index == LLPOSITION_FRONT || pos_index == 0)
    {
        return list_ptr->ll_front->data_ptr;
    }
    else if (pos_index == LLPOSITION_BACK || pos_index == list_ptr->ll_entry_count - 1)
    {
        return list_ptr->ll_back->data_ptr;
    }
    else if (pos_index < 0 || pos_index >= list_ptr->ll_entry_count)
        return NULL; // does not correspond to position in list

    // loop through the list until find correct position index
    for (rover = list_ptr->ll_front; pos_index > 0; pos_index--)
    {
        rover = rover->ll_next;
    }
    // already verified that pos_index should be valid so rover better not be null
    assert(rover != NULL);
    assert(rover->data_ptr != NULL);
    return rover->data_ptr;
}

/* Allocates a new, empty list
 *
 * If the comparison function is NULL, then the list is unsorted.
 *
 * Otherwise, the list is initially assumed to be sorted.  Note that if
 * list_insert is used the list is changed to unsorted.
 *
 * The field sorted can only take values LLIST_SORTED or LLIST_UNSORTED
 *
 * Use llist_destruct to remove and deallocate all elements on a list
 * and the header block.
 *
 * (This function is written and no changes needed. It provides an example
 *  of how save the comparison function pointer.  See other examples in this
 *  file for how to use compare_fun.)
 */
llist_t *llist_construct(int (*fcomp)(const data_t *, const data_t *))
{
    llist_t *new_list;
    new_list = (llist_t *)malloc(sizeof(llist_t));
    new_list->ll_front = NULL;
    new_list->ll_back = NULL;
    new_list->ll_entry_count = 0;
    new_list->compare_fun = fcomp;
    if (fcomp == NULL)
        new_list->ll_sorted_state = LLIST_UNSORTED;
    else
        new_list->ll_sorted_state = LLIST_SORTED;

    llist_debug_validate(new_list);
    return new_list;
}

/* Finds an element in a list and returns a pointer to it.
 *
 * list_ptr: pointer to list-of-interest.
 *
 * elem_ptr: element against which other elements in the list are compared.
 *
 * fcomp: function to test if elem_ptr is a match to an item in the list.
 *        Returns 0 if they match and any value not equal to 0 if they do not match.
 *
 * NOTICE: pos_index is returned and is not an input value!
 *
 * The function returns a pointer to the matching element with lowest index if
 * a match if found.  If a match is not found the return value is NULL.
 *
 * The function also returns the integer position of matching element with the
 *           lowest index.  If a matching element is not found, the position
 *           index that is returned should be -1.
 *
 * pos_index: used as a return value for the position index of matching element
 *
 */
data_t *llist_elem_find(llist_t *list_ptr, data_t *elem_ptr, int *pos_index,
                        int (*fcomp)(const data_t *, const data_t *))
{
    llist_debug_validate(list_ptr);

    *pos_index = 0;

    for (llist_elem_t *rover = list_ptr->ll_front; rover != NULL; rover = rover->ll_next)
    {
        if (fcomp(rover->data_ptr, elem_ptr) == 0)
        {
            // return the pointer, is lowest index because it's the first time we've found a match
            return rover->data_ptr;
        }
        (*pos_index)++;
    }
    *pos_index = -1;
    return NULL;
}

/* Deallocates the contents of the specified list, releasing associated memory
 * resources for other purposes.
 */
void llist_destruct(llist_t *list_ptr)
{
    // the first line must validate the list
    llist_debug_validate(list_ptr);

    for (llist_elem_t *rover = list_ptr->ll_front; rover != NULL;)
    {
        llist_elem_t *next = rover->ll_next;
        free(rover->data_ptr);
        free(rover);
        rover = next;
    }
}

/* Inserts the specified data element into the specified list at the specified
 * position.
 *
 * llist_ptr: pointer to list-of-interest.
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * pos_index: numeric position index of the element to be inserted into the
 *            list.  Index starts at 0 at head of the list, and incremented by
 *            one until the tail is reached.  The index can also be equal
 *            to LLPOSITION_FRONT or LLPOSITION_BACK (these are special negative
 *            values use to provide a short cut for adding to the head
 *            or tail of the list).
 *
 * If pos_index is greater than the number of elements currently in the list,
 * the element is simply appended to the end of the list (no additional elements
 * are inserted).
 *
 * Note that use of this function results in the list to be marked as unsorted,
 * even if the element has been inserted in the correct position.  That is, on
 * completion of this subroutine the llist_ptr->ll_sorted_state must be equal
 * to LLIST_UNSORTED.
 */
void llist_insert(llist_t *list_ptr, data_t *elem_ptr, int pos_index)
{
    assert(list_ptr != NULL);
    assert(pos_index == LLPOSITION_FRONT || pos_index == LLPOSITION_BACK || pos_index >= 0);
    // note you MUST handle the case that pos_index is LLPOSITION_FRONT or BACK

    if (pos_index == LLPOSITION_BACK || pos_index >= list_ptr->ll_entry_count - 1)
    {
        list_ptr->ll_back->ll_next = elem_ptr;
        elem_ptr = list_ptr->ll_back;
        list_ptr->ll_back = elem_ptr;
        list_ptr->ll_back->ll_next = NULL;
    }

    // the last three lines of this function must be the following
    if (list_ptr->ll_sorted_state == LLIST_SORTED)
        list_ptr->ll_sorted_state = LLIST_UNSORTED;
    llist_debug_validate(list_ptr);
}

/* Inserts the element into the specified sorted list at the proper position,
 * as defined by the compare_fun function pointer found in the list header:
 *     list_ptr->compare_fun(A, B)
 *
 * list_ptr: pointer to list-of-interest.
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * If you use llist_insert_sorted, the list preserves its sorted nature.
 *
 * If you use llist_insert, the list will be considered to be unsorted, even
 * if the element has been inserted in the correct position.
 *
 * If the list is not sorted and you call llist_insert_sorted, this subroutine
 * should generate a system error and the program should immediately stop.
 *
 * The comparison procedure must accept two arguments (A and B) which are both
 * pointers to elements of type data_t.  The comparison procedure returns an
 * integer code which indicates the precedence relationship between the two
 * elements.  The integer code takes on the following values:
 *    1: A should be closer to the list head than B
 *   -1: B should be closer to the list head than A
 *    0: A and B are equal in rank
 *
 * Note: if the element to be inserted is equal in rank to an element already
 *       in the list, the newly inserted element will be placed after all the
 *       elements of equal rank that are already in the list.
 */
void llist_insert_sorted(llist_t *list_ptr, data_t *elem_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->ll_sorted_state == LLIST_SORTED);

    // insert your code here

    // the last line checks if the new list is correct
    llist_debug_validate(list_ptr);
}

/* Removes an element from the specified list, at the specified list position,
 * and returns a pointer to the element.
 *
 * list_ptr: pointer to list-of-interest.
 *
 * pos_index: position of the element to be removed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LLPOSITION_FRONT and LLPOSITION_BACK
 *
 * Attempting to remove an element at a position index that is not contained in
 * the list will result in no element being removed, and a NULL pointer will be
 * returned.
 */
data_t *llist_remove(llist_t *list_ptr, int pos_index)
{
    assert(list_ptr != NULL);
    assert(pos_index == LLPOSITION_FRONT || pos_index == LLPOSITION_BACK || pos_index >= 0);
    // note you MUST handle the case that pos_index is LLPOSITION_FRONT or BACK

    // insert your code here

    // the last line should verify the list is valid after the remove
    llist_debug_validate(list_ptr);
    return NULL; // you must fix the return value
}

/* Obtains the length of the specified list, that is, the number of elements
 * that the list contains.
 *
 * list_ptr: pointer to list-of-interest.
 *
 * Returns an integer equal to the number of elements stored in the list.  An
 * empty list has a size of zero.
 *
 * (This function is already written, so no changes necessary.)
 */
int llist_entries(llist_t *list_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->ll_entry_count >= 0);
    return list_ptr->ll_entry_count;
}

/* This function verifies that the pointers for the two-way linked list are
 * valid, and that the list size matches the number of items in the list.
 *
 * If the linked list is sorted it also checks that the elements in the list
 * appear in the proper order.
 *
 * The function produces no output if the two-way linked list is correct.  It
 * causes the program to terminate and print a line beginning with "Assertion
 * failed:" if an error is detected.
 *
 * The checks are not exhaustive, so an error may still exist in the
 * list even if these checks pass.
 *
 * YOU MUST NOT CHANGE THIS FUNCTION.  WE USE IT DURING GRADING TO VERIFY THAT
 * YOUR LIST IS CONSISTENT.
 */
void llist_debug_validate(llist_t *L)
{
    llist_elem_t *N;
    int count = 0;
    assert(L != NULL);
    if (L->ll_front == NULL)
        assert(L->ll_back == NULL && L->ll_entry_count == 0);
    if (L->ll_back == NULL)
        assert(L->ll_front == NULL && L->ll_entry_count == 0);
    if (L->ll_entry_count == 0)
        assert(L->ll_front == NULL && L->ll_back == NULL);
    if (L->ll_entry_count == 1)
    {
        assert(L->ll_front == L->ll_back && L->ll_front != NULL);
        assert(L->ll_front->ll_next == NULL && L->ll_front->ll_prev == NULL);
        assert(L->ll_front->data_ptr != NULL);
    }
    if (L->ll_front == L->ll_back && L->ll_front != NULL)
        assert(L->ll_entry_count == 1);
    assert(L->ll_sorted_state == LLIST_SORTED || L->ll_sorted_state == LLIST_UNSORTED);
    if (L->ll_entry_count > 1)
    {
        assert(L->ll_front != L->ll_back && L->ll_front != NULL && L->ll_back != NULL);
        N = L->ll_front;
        assert(N->ll_prev == NULL);
        while (N != NULL)
        {
            assert(N->data_ptr != NULL);
            if (N->ll_next != NULL)
                assert(N->ll_next->ll_prev == N);
            else
                assert(N == L->ll_back);
            count++;
            N = N->ll_next;
        }
        assert(count == L->ll_entry_count);
    }
    if (L->ll_sorted_state == LLIST_SORTED && L->ll_front != NULL)
    {
        N = L->ll_front;
        while (N->ll_next != NULL)
        {
            assert(L->compare_fun(N->data_ptr, N->ll_next->data_ptr) != -1); // A <= B
            N = N->ll_next;
        }
    }
}
/* commands for vim. ts: tabstop, sts: softtabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
