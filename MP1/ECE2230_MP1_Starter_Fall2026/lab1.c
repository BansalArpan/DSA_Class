/* lab1.c template
 * Arpan Bansal                <<-- replace with your name!
 * abansal                      <<-- replace with your user name!
 * ECE 2230 Fall 2026
 * MP1
 *
 * NOTE:  You must update all of the following comments!
 *
 * Purpose: A template for MP1
 *
 * Assumptions: Many details are incomplete.  The functions to collect input
 * for a record and to print a record specify the format that is required for
 * grading.
 *
 * The program accepts one command line arguement that is the size of the list.
 *
 * An outline for the interactive menu input is provided.  Details need to be
 * completed but the format of the commands and the prints found in
 * ids_record_fill ids_print_rec should not be changed.
 *
 * Bugs: Many detail have not been implemented.
 *
 * See the ECE 2230 programming guide
 *
 * NOTE: if it forbidden to access any of the members in the ids_database
 * structure.   The member names MUST NOT be found in this file or it is a
 * design violation.  Instead you must utilize the ids_ fuctions found
 * in the ids.h header file to access any details of the list.
 *
 * One of the requirements is to verify you program does not have any
 * memory leaks or other errors that can be detected by valgrind.  Run with
 * your test scripts:
 *      valgrind --leak-check=full ./lab1 < your_test_script
 *
 * Are you unhappy with the way this code is formatted?  You can easily
 * reformat (and automatically indent) your code using the astyle
 * command.  If it is not installed use the Ubuntu Software Center to
 * install astyle.  Then in a terminal on the command line do
 *     astyle --style=kr lab1.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux, or google to see different options.  Or, set up
 * your own style.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "ids.h"

// this is a private function used for grading.  Do not change
void grading_db_size(struct ids_database *my_db_ptr, int old_size, int init_size);

int main(int argc, char **argv)
{
    struct alert_t *new_rec;
    char line[MAXLINE];
    char command[MAXLINE];
    char junk[MAXLINE];
    int num_items;
    int input_number;
    int list_size = -1;

    if (argc != 2)
    {
        printf("Usage: ./lab1 list_size\n");
        exit(1);
    }
    list_size = atoi(argv[1]);
    if (list_size < 1)
    {
        printf("lab1 has invalid list size: %d\n", list_size);
        exit(2);
    }
    printf("Welcome to lab1. Using list size: %d\n", list_size);
    printf("ALERT\nLISTGEN x, LISTIP x\nDELGEN x, DELIP x\nSCANGEN x\nPRINT\nQUIT\n");

    // fix.  Don't forget to create the database using the ids_construct command
    struct ids_database *mydb = ids_construct(list_size);

    // remember fgets includes newline \n unless line too long
    while (fgets(line, MAXLINE, stdin) != NULL)
    {
        num_items = sscanf(line, "%s%d%s", command, &input_number, junk);
        if (num_items == 1 && strcmp(command, "QUIT") == 0)
        {
            /* found exit */
            printf("cleanup\n");
            ids_destruct(mydb);
            break;
        }
        else if (num_items == 1 && strcmp(command, "ALERT") == 0)
        {
            new_rec = (struct alert_t *)malloc(sizeof(struct alert_t));
            ids_record_fill(new_rec);

            // you have to figure out what goes here
            // and call the correct printf command
            int add_return = ids_add(mydb, new_rec);

            if (add_return == 1)
            {
                printf("\nAdded: %d and doubled list size\n", new_rec->generator_id);
            }
            else if (add_return == 0)
            {
                printf("\nAdded: %d\n", new_rec->generator_id);
            }
            else
            {
                printf("\n\nError with ids_add return value\n");
                exit(1);
            }
            new_rec = NULL;
        }
        else if (num_items == 2 && strcmp(command, "LISTGEN") == 0)
        {

            int found = 0;
            int db_size = ids_count(mydb);
            for (int i = 0; i < db_size; i++)
            {
                if (ids_access(mydb, i)->generator_id == input_number)
                {
                    new_rec = ids_access(mydb, i);
                    // First, print each of the matching alerts and count the number found
                    // fix
                    ids_print_rec(new_rec);
                    found++;
                }
            }
            // once the number of matches is found, print message
            if (found == 0)
            {
                printf("Did not find alert: %d\n", input_number);
            }
            else
            {
                printf("Found %d matching generator alerts %d\n", found, input_number);
            }
            new_rec = NULL;
        }
        else if (num_items == 2 && strcmp(command, "LISTIP") == 0)
        {
            int found = 0;
            int db_size = ids_count(mydb);
            for (int i = 0; i < db_size; i++)
            {
                if (ids_access(mydb, i)->dest_ip_addr == input_number)
                {
                    new_rec = ids_access(mydb, i);
                    // First, print each of the matching alerts and count the number found
                    ids_print_rec(new_rec);
                    found++;
                }
            }

            // once the number of matches is found, print message
            if (found == 0)
            {
                printf("Did not find destination IP: %d\n", input_number);
            }
            else
            {
                printf("Found %d alerts matching IP %d\n", found, input_number);
            }
            new_rec = NULL;
        }
        else if (num_items == 2 && strcmp(command, "DELGEN") == 0)
        {
            int old_db_size = ids_size(mydb); // before any changes save size of database
            int found = 0;
            // int db_count = ids_count(mydb);
            int i = 0;
            while (i < ids_count(mydb))
            {
                if (ids_access(mydb, i)->generator_id == input_number)
                {
                    free(ids_remove(mydb, i));
                    found++;
                }
                else
                {
                    i++;
                }
            }

            if (found == 0)
            {
                printf("Did not remove alert type: %d\n", input_number);
                assert(old_db_size == ids_size(mydb));
            }
            else
            {
                printf("Removed %d matching generator alerts %d\n", found, input_number);
                // But, do not print each packet
                // this function is for grading.  Do not change it
                grading_db_size(mydb, old_db_size, list_size);
            }
        }
        else if (num_items == 2 && strcmp(command, "DELIP") == 0)
        {
            int old_db_size = ids_size(mydb); // before any changes save size of database
            int found = 0;
            // int db_count = ids_count(mydb);
            int i = 0;
            while (i < ids_count(mydb))
            {
                if (ids_access(mydb, i)->dest_ip_addr == input_number)
                {
                    free(ids_remove(mydb, i));
                    found++;
                }
                else
                {
                    i++;
                }
            }
            if (found == 0)
            {
                printf("Did not remove any alert with IP: %d\n", input_number);
            }
            else
            {
                printf("Removed %d alerts matching IP %d\n", found, input_number);
                // But, do not print each packet
                // this function is for grading.  Do not change it
                grading_db_size(mydb, old_db_size, list_size);
            }
        }
        else if (num_items == 2 && strcmp(command, "SCANGEN") == 0)
        {
            // for the SCANGEN command the input_number is the threshold
            // and is stored in input_number
            int matches = 0;
            int gen_id = 0;
            int groups = 0;

            int db_size = ids_count(mydb);
            // loop to find gen_ids with input_number or more matches
            for (int i = 0; i < db_size; i++)
            {
                if (ids_access(mydb, i) == NULL)
                {
                    break;
                }
                if (matches == 0)
                {
                    gen_id = ids_access(mydb, i)->generator_id;
                }
                if (i + 1 < db_size && ids_access(mydb, i)->generator_id == (ids_access(mydb, i + 1)->generator_id))
                {
                    matches++;
                }
                else
                {
                    matches++;
                    if (matches >= input_number)
                    {
                        groups++;
                        // for each group that is found print the number of matches
                        printf("A set with generator %d has %d alerts\n", gen_id, matches);
                    }
                    gen_id = 0;
                    matches = 0;
                }
            }

            // after all sets have been found print how many sets
            if (groups > 0)
            {
                printf("Found %d sets with at least %d matches\n", groups, input_number);
            }
            else
            {
                printf("No records with >= %d matches\n", input_number);
            }
        }
        else if (num_items == 1 && strcmp(command, "PRINT") == 0)
        {
            int num_in_list = ids_count(mydb); // fix!
            int array_size = ids_size(mydb);   // fix!
            if (num_in_list == 0)
            {
                printf("List empty. DB size is %d\n", array_size);
            }
            else
            {
                printf("List has %d records. DB size is %d\n", num_in_list, array_size);
                int i;
                for (i = 0; i < num_in_list; i++)
                {
                    printf("%d: ", i + 1);
                    // you must use the ids_print_rec function to format output
                    new_rec = ids_access(mydb, i);
                    ids_print_rec(new_rec);
                }
            }
            new_rec = NULL;
        }
        else
        {
            printf("# %s", line);
        }
    }
    exit(0);
}

/* this is a function for grading.  It checks if the size of the database was
 * correctly reduced after items were deleted.
 *
 * DO NOT CHANGE THIS FUNCTION.   It is used during grading to verify resizing
 * of the list is working
 */
void grading_db_size(struct ids_database *my_db_ptr, int old_size, int init_size)
{
    int alert_count = ids_count(my_db_ptr);
    int new_db_size = ids_size(my_db_ptr);
    if (new_db_size < old_size)
    {
        printf("    Reduced size of database to %d\n", new_db_size);
        if (new_db_size < init_size)
        {
            printf("   ERROR, the size of database reduced too much.  Must be at least %d\n", init_size);
            assert(new_db_size >= init_size);
        }
    }
    else
    {
        assert(new_db_size == old_size);
        if (alert_count < new_db_size / 5.0 && new_db_size >= 2 * init_size)
        {
            printf("   ERROR, did not reduce size of database.  Count=%d, db size = %d\n", alert_count, new_db_size);
            assert(alert_count >= new_db_size / 5.0 || new_db_size < 2 * init_size);
        }
    }
}

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
