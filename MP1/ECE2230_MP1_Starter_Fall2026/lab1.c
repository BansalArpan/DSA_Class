/* lab1.c
 * Arpan Bansal
 * abansal
 * ECE 2230 Fall 2026
 * MP1
 *
 * Purpose: Read commands to add, find, delete, scan, and print alert records.
 * Use the database functions to manage storage and preserve ordering.
 *
 * Usage: ./lab1 list_size, where list_size is a positive integer capacity.
 * Assumptions: Input contains complete commands and valid integer record
 * fields. Allocations are successful, and a normal session ends with QUIT.
 *
 * Bugs: No known bugs.
 *
 * The driver must use the public interface to access database state.
 * The supplied record input/output formats and grading function are retained.
 * Example memory check:
 * valgrind --leak-check=full ./lab1 2 < myinput.txt
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

    // Create the empty database using the requested initial capacity.
    struct ids_database *mydb = ids_construct(list_size);

    // remember fgets includes newline \n unless line too long
    while (fgets(line, MAXLINE, stdin) != NULL)
    {
        num_items = sscanf(line, "%s%d%s", command, &input_number, junk);
        if (num_items == 1 && strcmp(command, "QUIT") == 0)
        {
            /* Free all remaining records before leaving the command loop. */
            printf("cleanup\n");
            ids_destruct(mydb);
            break;
        }
        else if (num_items == 1 && strcmp(command, "ALERT") == 0)
        {
            new_rec = (struct alert_t *)malloc(sizeof(struct alert_t));
            ids_record_fill(new_rec);

            // A successful addition adds the record to the database.
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
                    // borrows the record without removing or freeing it.
                    ids_print_rec(new_rec);
                    found++;
                }
            }
            // Report the number of matching records, including the no-match case.
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
                    // Print each matching record and count it without changing the list.
                    ids_print_rec(new_rec);
                    found++;
                }
            }

            // Report the number of matching records, including the no-match case.
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
            int old_db_size = ids_size(mydb); // Save capacity for the grading check.
            int found = 0;
            int i = 0;
            // Removal shifts records left.
            while (i < ids_count(mydb))
            {
                if (ids_access(mydb, i)->generator_id == input_number)
                {
                    // Removes the record and frees the memory for it.
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
                // Report capacity changes using the supplied grading function.
                grading_db_size(mydb, old_db_size, list_size);
            }
        }
        else if (num_items == 2 && strcmp(command, "DELIP") == 0)
        {
            int old_db_size = ids_size(mydb); // Save capacity for the grading check.
            int found = 0;
            int i = 0;
            // Removal shifts records left.
            while (i < ids_count(mydb))
            {
                if (ids_access(mydb, i)->dest_ip_addr == input_number)
                {
                    // Removes the record and frees the memory for it.
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
                // Report capacity changes using the supplied grading function.
                grading_db_size(mydb, old_db_size, list_size);
            }
        }
        else if (num_items == 2 && strcmp(command, "SCANGEN") == 0)
        {
            // input_number is the threshold to report.
            int matches = 0;
            int gen_id = 0;
            int groups = 0;

            int db_size = ids_count(mydb);
            // Equal generator IDs are consecutive, so count one group at a time.
            for (int i = 0; i < db_size; i++)
            {
                if (ids_access(mydb, i) == NULL)
                {
                    break;
                }
                // Capture the first ID even when the group contains one record.
                if (matches == 0)
                {
                    gen_id = ids_access(mydb, i)->generator_id;
                }
                // Check the bound before reading the next record.
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
                        // The group is complete; report it if it meets the threshold.
                        printf("A set with generator %d has %d alerts\n", gen_id, matches);
                    }
                    gen_id = 0;
                    matches = 0;
                }
            }

            // Summarize the number of qualifying groups.
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
            int num_in_list = ids_count(mydb); // Occupied entries
            int array_size = ids_size(mydb);   // Allocated capacity
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
                    // Display positions start at 1; database indices start at 0.
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
