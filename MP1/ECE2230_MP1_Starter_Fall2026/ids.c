/* ids.c template
 * Arpan Bansal        <<-- you must change these details!
 * ECE 2230 Fall 2026
 * MP1
 *
 * Purpose: An Intrusion Detection System template for MP1
 *
 * Assumptions: Many details are incomplete.  The functions to collect input
 * for a record and to print a record specify the format that is required for
 * grading.
 *
 * Bugs: Many details have not been implemented.
 *
 * You must add comments for each function
 *
 * See the ECE 2230 programming guide
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "ids.h"

struct ids_database *ids_construct(int size)
{
    if (size <= 0)
    {
        return NULL;
    }
    struct ids_database *new_database = malloc(sizeof(struct ids_database));
    new_database->alert_ptr = malloc(size * sizeof(struct alert_t *));
    //(struct ids_database *)malloc(size * sizeof(struct alert_t *));
    for (int i = 0; i < size; i++)
    {
        (new_database->alert_ptr)[i] = NULL;
    }
    new_database->db_entries = 0;
    new_database->db_size = size;
    new_database->init_size = size;
    return new_database;
}

void ids_destruct(struct ids_database *list)
{
    if (list == NULL)
    {
        return;
    }

    int size = list->db_entries;

    for (int i = 0; i < size; i++)
    {
        free(list->alert_ptr[i]);
    }
    free(list->alert_ptr);
    free(list);
}

int ids_add(struct ids_database *list, struct alert_t *rec_ptr)
{
    if (list == NULL || rec_ptr == NULL)
    {
        return -99;
    }
    int retVal = 0;
    if (list->db_entries >= list->db_size)
    {
        struct alert_t **arr = realloc(list->alert_ptr, 2 * (list->db_size) * sizeof(*list->alert_ptr));
        if (arr == NULL)
        {
            return -99;
        }
        list->alert_ptr = arr;
        list->db_size *= 2;
        retVal = 1;
    }
    //    struct alert_t curr = *(list->alert_ptr)[0]
    int loc = 0;
    while (loc < list->db_entries && list->alert_ptr[loc]->generator_id <= rec_ptr->generator_id)
    {
        loc++;
    }
    for (int i = list->db_entries; i > loc; i--)
    {
        list->alert_ptr[i] = list->alert_ptr[i - 1];
    }
    list->alert_ptr[loc] = rec_ptr;
    list->db_entries++;
    return retVal;
    /* the return codes
     *   0  list did not change size
     *   1  doubled list size
     */
}

struct alert_t *ids_access(struct ids_database *list, int index)
{
    if (list == NULL || index < 0 || index >= list->db_entries)
    {
        return NULL;
    }
    return (list->alert_ptr[index]);
}

struct alert_t *ids_remove(struct ids_database *list, int index)
{
    if (list == NULL || index < 0 || index >= list->db_entries)
    {
        return NULL;
    }

    struct alert_t *retVal = list->alert_ptr[index];
    if (retVal == NULL)
    {
        return NULL;
    }
    for (int i = index; i < list->db_entries - 1; i++)
    {
        list->alert_ptr[i] = list->alert_ptr[i + 1];
    }

    list->db_entries--;
    list->alert_ptr[list->db_entries] = NULL;
    if (list->db_entries < list->db_size * .2 && list->db_size / 2 >= list->init_size)
    {
        struct alert_t **arr = realloc(list->alert_ptr, (list->db_size / 2) * sizeof(*list->alert_ptr));
        if (arr == NULL)
        {
            return retVal;
        }
        list->alert_ptr = arr;
        list->db_size /= 2;
    }

    return retVal;
}

int ids_empty(struct ids_database *list)
{
    if (list->alert_ptr[0] != NULL)
    {
        return 0;
    }
    return list->db_entries == 0;
}

int ids_count(struct ids_database *list)
{
    if (list == NULL)
    {
        return -1;
    }
    return list->db_entries;
}

int ids_size(struct ids_database *list)
{
    if (list == NULL)
    {
        return -1;
    }
    return list->db_size;
}

/* Prompts user for alert record input starting with the source IP address.
 * The input is not checked for errors but will default to an acceptable value
 * if the input is incorrect or missing.
 *
 * The input to the function assumes that the structure has already been
 * created.  The contents of the structure are filled in.
 *
 * There is no output.
 *
 * Do not change the sequence of prompts as grading requires this exact
 * format
 */
void ids_record_fill(struct alert_t *rec)
{
    char line[MAXLINE];
    assert(rec != NULL);

    printf("Generator component:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->generator_id);
    printf("Signature:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->signature_id);
    printf("Revision:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->revision_id);
    printf("Dest IP address:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->dest_ip_addr);
    printf("Source IP address:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->src_ip_addr);
    printf("Destination port number:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->dest_port_num);
    printf("Source port number:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->src_port_num);
    printf("Time:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->timestamp);
    printf("\n");
}

/* print the information for a particular alert record
 *
 * Input is a pointer to a record, and no entries are changed.
 *
 * Do not change any of these lines and grading depends on this
 * format.
 */
void ids_print_rec(struct alert_t *rec)
{
    assert(rec != NULL);
    printf("[%d:%d:%d] (gen, sig, rev): ", rec->generator_id, rec->signature_id,
           rec->revision_id);
    printf("Dest IP: %d, Src: %d, Dest port: %d,", rec->dest_ip_addr,
           rec->src_ip_addr, rec->dest_port_num);
    printf(" Src: %d, Time: %d\n", rec->src_port_num, rec->timestamp);
}

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
