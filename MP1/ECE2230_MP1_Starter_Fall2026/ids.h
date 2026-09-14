/* ids.h template
 * Arpan Bansal
 * ECE 2230 Fall 2026
 * MP1
 *
 * Purpose: A template for MP1
 *          The only changes permitted are to the ids_database structure
 *
 * Assumptions: the alert_t structure and the public functions as defined for
 * this assignmnet and cannot be changed.
 *
 * Bugs:
 *  No known bugs.
 *
 */

#define MAXLINE 100

// You can add members to this structure if needed (such as initial size)
struct ids_database
{
    int db_size;    // size of array
    int db_entries; // current count of number of alert records in the list
    struct alert_t **alert_ptr;
    int init_size;
};
struct alert_t
{
    int generator_id;  // ID of component generating alert
    int signature_id;  // ID of detection rule
    int revision_id;   // revision number of detection rule
    int dest_ip_addr;  // IP address of destination
    int src_ip_addr;   // IP address of source
    int dest_port_num; // port number at destination
    int src_port_num;  // port number at source host
    int timestamp;     // time in seconds alert received
};

//  The sequential list ADT must have the following interface.  You
//  cannot change the interface for any of these functions

struct ids_database *ids_construct(int);
void ids_destruct(struct ids_database *);
int ids_add(struct ids_database *, struct alert_t *);
struct alert_t *ids_access(struct ids_database *, int);
struct alert_t *ids_remove(struct ids_database *, int);
int ids_empty(struct ids_database *);
int ids_count(struct ids_database *);
int ids_size(struct ids_database *);
void ids_record_fill(struct alert_t *);
void ids_print_rec(struct alert_t *);

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
