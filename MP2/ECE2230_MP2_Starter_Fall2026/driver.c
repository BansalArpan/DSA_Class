/* driver.c

Writing and debugging MP2 is challenging, and it is particularly hard if you
try and write all the code before you do any testing. It is much easier to
break the problem into small pieces and test each piece.

For MP2, one approach is to write llist_insert first, since it might be the
easiest algorithm and it depends only on llist_construct.

   For testing, we use a simple main function in this file.  This file is for
   testing only, and you will NOT submit it as it is for your personal testing
   during development.

Below are a few simple examples that allocates a few structures and inserts 
them into a list.

With this file, you can compile and run llist.c on its own using 

   gcc -Wall -g driver.c llist.c ids_support.c -o driver

The compiling command is also in the makefile. Do
   make driver 

Do this testing before you make ANY changes to ids_support.c.  The only 
reason we need to compile ids_support.c is for the ids_compare function.
The linker needs to find the compare_fun function and it is found in the template
   ids_support.c file (with no changes) 
*/

#include <stdlib.h>
#include <stdio.h>
#include "llist.h"
#include "ids_support.h"

/* prototype for list debug print */
void llist_debug_print(llist_t *list_ptr);

int main(void)
{
    data_t *datap = NULL;
    llist_t *unsorted_list = NULL;

    unsorted_list = llist_construct(ids_compare_genid);

    printf("\nFirst test of unsorted insert.  Should insert 555\n\n");
    // create one item to test llist_insert
    datap = (data_t *) calloc(1, sizeof(data_t));
    datap->generator_id = 555;
    llist_insert(unsorted_list, datap, LLPOSITION_FRONT);
    datap = NULL;

    // test llist_access with one item in list
    datap = llist_access(unsorted_list, LLPOSITION_FRONT);
    if (datap != NULL) {
        printf("access looked for first item and found (%d)\n", datap->generator_id);
    } else {
        printf("ERROR: list access is broken?\n");
    }
    datap = NULL;

    // add a second item  to tail of the list
    datap = (data_t *) calloc(1, sizeof(data_t));
    datap->generator_id = 333;
    llist_insert(unsorted_list, datap, LLPOSITION_BACK);
    datap = NULL;

    // add a third item in the middle
    datap = (data_t *) calloc(1, sizeof(data_t));
    datap->generator_id = 444;
    llist_insert(unsorted_list, datap, 1);
    datap = NULL;

    printf("\nSecond test. Inserted 333 and 444.  Use access to find them. List should be {555, 444, 333}\n\n");
    // find all three and print 
    datap = llist_access(unsorted_list, 0);   /* same as LLPOSITION_FRONT */
    if (datap != NULL) {
        printf("Position 0 should find 555 and found (%d)\n", datap->generator_id);
        datap = llist_access(unsorted_list, 1);
        printf("Position 1 should find 444 and found (%d)\n", datap->generator_id);
        datap = llist_access(unsorted_list, LLPOSITION_BACK);
        printf("Position 2 should find 333 and found (%d)\n", datap->generator_id);
    } else {
        printf("ERROR: list access is broken?\n");
    }

    //Next try to use llist_debug_print 
    printf("\nRepeat prints to test the list debug print function.  It should print same list.\n\n");
    llist_debug_print(unsorted_list);

    // you should repeat the above tests with differents insertion orders

    //*********************************************************************
    // next you may want to work on llist_insert_sorted, 
 
    printf("\nNext test using new list and sorted inserts.  About to insert 31, 7, 15, and 63.\n\n");
    llist_t *Lsortptr = llist_construct(ids_compare_genid);

    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->generator_id = 31;
    llist_insert_sorted(Lsortptr, datap);

    // add a second item in front
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->generator_id = 7;
    llist_insert_sorted(Lsortptr, datap);

    // add a third item in middle
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->generator_id = 15;
    llist_insert_sorted(Lsortptr, datap);

    // add a fourth item to end
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->generator_id = 63;
    llist_insert_sorted(Lsortptr, datap);

    printf("\nHere is the sorted list. The print should show {7, 15, 31, 63}\n\n");
    llist_debug_print(Lsortptr);
 
    // after the insertion functions work, you can try the find command
 
    printf("\nTest of list elem find position on the unsorted list\n");
    int position = -1;
    data_t my_template;
    my_template.generator_id = 444;
    data_t *foundp = llist_elem_find(unsorted_list, &my_template, &position, ids_compare_genid);
    if (position != -1) {
        printf("Alert %d found in position %d\n", my_template.generator_id, position);
        data_t *foundp2 = llist_access(unsorted_list, position);
        if (foundp != NULL && foundp2 != NULL && foundp == foundp2) {
            printf("looked for %d and found %d in position %d\n",
                    my_template.generator_id, foundp->generator_id, position);
            if (my_template.generator_id != foundp->generator_id) {
                printf("ERROR: returned pointer to wrong record\n");
            }
        } else {
            printf("ERROR: access did not get position %d\n", position);
        }
    } else {
        printf("ERROR: looked for %d and did not find it!\n", my_template.generator_id);
    }

    // *************************************************************/
    printf("\n\nHow are you going to test the remove function?\n");
    //  you should add tests here

 
    // when done with testing clean up
    llist_destruct(unsorted_list);
    llist_destruct(Lsortptr);
    return 0;
}

/*

   Next you will want to write your own llist_debug_print function to print a
   list. Then you can do "before and after" testing. That is, print the list
   before a change, and print the list after the change to verify that the change
   worked.

   The following is an example and is nearly identical to the version
   found in ex3.c (which we studied in class).
*/

void llist_debug_print(llist_t *list_ptr) {
    llist_elem_t *rover;
    int count;

    printf("list size: %d\n", list_ptr->ll_entry_count);
    rover = list_ptr->ll_front;
    count = 0;
    while(rover != NULL) {
        printf("Pos: %d contains ID: %d\n", count, rover->data_ptr->generator_id);
        rover = rover->ll_next;
        count++;
    }
    if (count != list_ptr->ll_entry_count)
        printf("\nERROR: the list size is wrong! count is %d\n", count);
}

