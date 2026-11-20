/* lab5.c 
 * My Name is
 * My User ID is
 * Lab5: Binary Search Trees
 * ECE 2230, Fall 2026
 *
 * This file contains drivers to test the BST package.
 *
 * The unit driver builds a custom tree and removes selected keys 
 *   -u 1:  run the unit driver with a custom tree
 *   The provided unit drivers are numbered 0 to 8; add your own cases.  
 *
 * There are three drivers to build a tree using tree_insert and then search 
 * for keys in the tree using tree_search.  Use
 *   -o run the driver with an optimal tree
 *   -r run the driver with a randomly generated tree
 *   -p run the driver with a poor order for inserting keys 
 *   -w to set the number of levels in the initial tree
 *   -t to set the number of search trials 
 *
 *   -v to turn on verbose prints.  But make -w and -t small: example -w4 -t10
 *
 * Another test driver tests random inserts and deletes.  This driver builds
 * an initial tree that is random, and then performs insertions and deletions
 * with equal probability.
 *   -e run the equilibrium driver
 *
 * Use -h or --help for options and defaults. POSIX environment required.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>

#include "tree.h"   // change this to match the name of your header file

/* constants used with Global variables */

#define TRUE 1
#define FALSE 0

/* Global variables for command line parameters */
int Seed = 11082026;
int Verbose = FALSE;
int EquilibriumTest = FALSE;
int OptimalTest = FALSE;
int RandomTest = FALSE;
int PoorTest = FALSE;
int UnitNumber = -1;
int Levels = 16;
int Trials = 100000;

enum testtypes {OPTIMAL, RANDOM, POOR};

/* prototypes for functions in this file only */
void getCommandLine(int argc, char **argv);
void equilibriumDriver(void);
void search_driver(int);
void unitDriver(const int *, const int, const int *, const int);
void big_test(void);

void build_optimal(tree_t *T, int levels);
void build_random(tree_t *T, int levels);
void build_poor(tree_t *T, int levels);

int main(int argc, char **argv)
{
    getCommandLine(argc, argv);
    printf("Seed: %d\n", Seed);
    srand48(Seed);

    /* ----- build custom tree and remove nodes ----- */
    if (UnitNumber == 0)                        /* enabled with -u flag */
    {
	// example test to remove leaves, 12 and 20, then internal nodes
	// 8, 24, 40 with one child, then 16, 48 with two children
	const int ins[] = {32,16,8,24,4,12,20,28,48,40,56,44,42,46};
	const int del[] = {12,20,8,24,40,16,48}; 
        unitDriver(ins, sizeof ins / sizeof(int),
		   del, sizeof del / sizeof(int));
    }
    if (UnitNumber == 1)
    {
	// example tests: (48) is missing its right-left child and
	//                (16) is missing its left-right child
	const int ins1[] = {32,16,48,8,24,40,56,4,20,28,36,44,60};
	const int del1[] = {16,48,32}; 
        unitDriver(ins1, sizeof ins1 / sizeof(int),
		   del1, sizeof del1 / sizeof(int));
    }
    if (UnitNumber == 2)
    {
	// example tests: (16) is missing its right-left child and 
	//                (48) is missing its left-right child
	const int ins1b[] = {32,16,48,8,24,40,56,4,12,28,36,52,60};
	const int del1b[] = {16,48,32}; 
        unitDriver(ins1b, sizeof ins1b / sizeof(int),
		   del1b, sizeof del1b / sizeof(int));
    }
    if (UnitNumber == 3)
    {
	// example deletion with many children
	const int ins[] = {200,100,50,150,25,75,125,175,65,85,135,80,130,140,78,82};
	const int del[] = {100,85,125}; 
        unitDriver(ins, sizeof ins / sizeof(int),
		   del, sizeof del / sizeof(int));
    }
    if (UnitNumber == 4)
    {
        // check replace for duplicate key
        const int ins[] = {10, 10};
	const int del[] = {10};
        unitDriver(ins, sizeof ins/sizeof(int), del, sizeof del/sizeof(int));
    }
    if (UnitNumber == 5)
    {
        // deleting a key that was never inserted must return NULL
        const int ins[] = {14, 6, 20, 12, 1, 5, 4, 22, 24, 26, 28, 30};
	const int del[] = {10};
        unitDriver(ins, sizeof ins/sizeof(int), del, sizeof del/sizeof(int));
    }
    if (UnitNumber == 6)
    {
        // example with poor shape 
        const int ins[] = {10, 8, 26, 6, 14, 28, 4, 20, 30, 2, 32, 16, 18, 12, 22, 24, 34};
	const int del[] = {10};
        unitDriver(ins, sizeof ins/sizeof(int), del, sizeof del/sizeof(int));
    }
    if (UnitNumber == 7)
    {
        if (Levels > 4) {
            printf("\n\tThis unit driver tests searches and number of comparisons\n");
            printf("\tRun with -u7 -w x for x in the range 0,1,2,3, or 4\n");
            exit(1);
        }
        tree_t *T = tree_construct();
        assert(T != NULL);
        // build_optimal(T, Levels);
        build_random(T, Levels);
        int n = pow(2, Levels);
        tree_debug_print_tree(T);
        for (int key = 1; key < 2*n; key++) {
            int level = tree_level(T, key);
            data_t *dp = tree_search(T, key);
            int count = tree_stats(T);
            if (dp == NULL) {
                printf("key: %d not found on level %d, comparisons: %d", key, level, count);
                if (2*level != count) {
                    printf(" --- ERROR level and count inconsistent!\n");
                } else {
                    printf("\n");
                }
                if (key%2 == 0) {  // all even keys in the tree
                    printf("\n\n\t ERROR key %d not found but is in the tree!\n\n", key);
                }
            } else {
                printf("key: %d found on level %d, comparisons: %d", key, level, count);
                if (2*level + 1 != count) {
                    printf(" --- ERROR level and count inconsistent!\n");
                } else {
                    printf("\n");
                }
                if (key%2 == 1) {  // all odds keys not in the tree
                    printf("\n\n\t ERROR key %d found but is not in the tree!\n\n", key);
                }
            }
            assert(count == (dp != NULL ? 2 * level + 1 : 2 * level));
            assert((dp != NULL) == (key % 2 == 0));
        }
        tree_destruct(T);
    }
    if (UnitNumber == 8)
    {
        // Additional deletion cases: leaves, one child, and two children.
        big_test();
    }

    /* ----- large tree tests  ----- */
    if (OptimalTest)                       /* enable with -o flag */
        search_driver(OPTIMAL);
    if (RandomTest)                        /* enable with -r flag */
        search_driver(RANDOM);
    if (PoorTest)                          /* enable with -p flag */
        search_driver(POOR);

    /* test for performance in equilibrium */
    if (EquilibriumTest)                   /* enable with -e flag */
        equilibriumDriver();

    return 0;
}

/* Unit tests know the intended membership independently of the BST.  This
 * also handles duplicate insertions and repeated or missing-key deletions.
 */
static int key_in_array(const int keys[], int count, int key)
{
    for (int i = 0; i < count; i++)
        if (keys[i] == key) return TRUE;
    return FALSE;
}

void unitDriver(const int ins_keys[], const int num_inserts,
        const int del_keys[], const int num_deletes)
{
    tree_t *del_tree = tree_construct();
    int expected_size = 0;
    assert(del_tree != NULL);
    assert(tree_size(del_tree) == 0);
    printf("\n\n ====== Unit Driver ======\n\n");
    printf("Inserting %d items into tree\n", num_inserts);
    for (int i = 0; i < num_inserts; i++) {
        int expected_new = !key_in_array(ins_keys, i, ins_keys[i]);
        int *ip = malloc(sizeof *ip);
        assert(ip != NULL);
        *ip = -ins_keys[i];
        int inserted = tree_insert(del_tree, ins_keys[i], ip);
        assert(inserted == expected_new);
        expected_size += expected_new;
        assert(tree_size(del_tree) == expected_size);
        printf(" -- Test (%d) tree after adding key %d\n", i, ins_keys[i]);
        tree_debug_print_tree(del_tree);
    }
    printf("\nAll insertions complete.\n");
    if (num_deletes > 0)
        printf("Removing %d items from tree\n", num_deletes);
    for (int i = 0; i < num_deletes; i++) {
        int expected_present = key_in_array(ins_keys, num_inserts, del_keys[i]) &&
                !key_in_array(del_keys, i, del_keys[i]);
        printf(" -- Test (%d) about to remove key %d\n", i, del_keys[i]);
        data_t *dp = tree_remove(del_tree, del_keys[i]);
        if (expected_present) {
            assert(dp != NULL);
            assert(*dp == -del_keys[i]);
            expected_size--;
            printf("    PASS: existing key removed\n");
        } else {
            assert(dp == NULL);
            printf("    PASS: missing key correctly returned NULL\n");
        }
        assert(tree_size(del_tree) == expected_size);
        tree_debug_print_tree(del_tree);
        free(dp);
    }
    tree_destruct(del_tree);
}

/* The expected unsuccessful count assumes equal probability for each of
 * the n+1 gaps.  The equilibrium driver's integer sampling can give those
 * gaps different probabilities after insertions and deletions.
 */
static void print_search_summary(int size, int ipl, long long suc_search,
        int suc_trials, long long unsuc_search, int unsuc_trials,
        int uniform_gap_benchmark)
{
    long long epl = (long long)ipl + 2LL * size;
    printf("  Expect successful search=");
    if (size > 0) printf("%g", (2.0 * ipl + size) / size);
    else printf("N/A (empty tree)");
    printf(", measured=");
    if (suc_trials > 0) printf("%g", (double)suc_search / suc_trials);
    else printf("N/A");
    printf(", trials=%d\n", suc_trials);
    printf("  %s unsuccessful search=%g, measured=",
            uniform_gap_benchmark ? "Uniform-gap benchmark" : "Expect",
            2.0 * epl / (size + 1));
    if (unsuc_trials > 0) printf("%g", (double)unsuc_search / unsuc_trials);
    else printf("N/A");
    printf(", trials=%d\n", unsuc_trials);
}

/* driver to build and test trees.  Creates tree with half of keys
 * in tree and half missing.  Uses search to find random keys.  Note that this
 * algorithm does not delete keys from the tree.
 */
void search_driver(int test_type)
{
    int i;
    int range_num_ints;
    int size;
    int key;
    int ipl;
    long long suc_search, unsuc_search;
    int suc_trials, unsuc_trials;
    tree_t *test_tree;
    data_t *dp;
    clock_t start, end;

    /* print parameters for this test run */
    printf("\n----- Access driver -----\n");
    printf("  Access trials: %d\n", Trials);
    printf("  Levels for tree: %d\n", Levels);

    range_num_ints = pow(2, Levels);

    /* build tree.  Key range is twice size of tree and each key 
     * is either even or odd.
     */
    test_tree = tree_construct();
    assert(test_tree != NULL);
    printf("  Build");
    if (test_type == OPTIMAL) {
        printf(" optimal");
        build_optimal(test_tree, Levels);
    } else if (test_type == RANDOM) {
        printf(" random");
        build_random(test_tree, Levels);
    } else if (test_type == POOR) {
        printf(" poor");
        build_poor(test_tree, Levels);

    } else {
        printf("invalid option in search test?\n");
        exit(1);
    }
    printf(" tree with size=%d\n", range_num_ints-1);

    if (Verbose)
        tree_debug_print_tree(test_tree);
    size = tree_size(test_tree);
    assert(size == range_num_ints-1);

    /* Search only: this phase must preserve the tree size. */
    suc_search = unsuc_search = 0;
    suc_trials = unsuc_trials = 0;
    start = clock();
    for (i = 0; i < Trials; i++) {
        key = ((int)(drand48() * (range_num_ints * 2 - 1))) + 1;
        dp = tree_search(test_tree, key);
        assert((dp != NULL) == (key % 2 == 0));
        if (dp == NULL) {
            unsuc_search += tree_stats(test_tree);
            unsuc_trials++;
            if (Verbose)
                printf("did not find %d after %d comparisons\n", key, tree_stats(test_tree));
        } else {
            suc_search += tree_stats(test_tree);
            suc_trials++;
            assert(*dp == -key);
            if (Verbose)
                printf("found %d after %d comparisons\n", key, tree_stats(test_tree));
        }
    }
    end = clock();
    assert(size == tree_size(test_tree));
    ipl = tree_internal_path_len(test_tree);
    printf("  After search exercise, time=%g ms, tree size=%d\n",
            1000 * ((double)(end - start)) / CLOCKS_PER_SEC, size);
    print_search_summary(size, ipl, suc_search, suc_trials,
            unsuc_search, unsuc_trials, FALSE);

    tree_debug_validate(test_tree);
    /* remove and free all items from tree */
    tree_destruct(test_tree);
    printf("----- End of search driver -----\n\n");
}
/* driver to test sequence of inserts and deletes.
 */
void equilibriumDriver(void)
{
    int i;
    int range_num_ints;
    int size;
    int key;
    int ipl;
    long long suc_search, unsuc_search;
    int suc_trials, unsuc_trials;
    int keys_added, keys_removed;
    int *ip;
    tree_t *test_tree;
    data_t *dp;
    clock_t start, end;

    /* print parameters for this test run */
    printf("\n----- Equilibrium test driver -----\n");
    printf("  Trials in equilibrium: %d\n", Trials);
    printf("  Levels in initial tree: %d\n", Levels);

    range_num_ints = pow(2, Levels);

    /* build a random tree as starting point */
    test_tree = tree_construct();
    assert(test_tree != NULL);
    build_random(test_tree, Levels);
    size = tree_size(test_tree);
    ipl = tree_internal_path_len(test_tree);
    printf("  Initial random tree size=%d\n", size);
    print_search_summary(size, ipl, 0, 0, 0, 0, TRUE);
    if (Verbose)
        tree_debug_print_tree(test_tree);

    /* in equilibrium make inserts and removes with equal probability */
    suc_search = unsuc_search = 0;
    suc_trials = unsuc_trials = 0;
    keys_added = keys_removed = 0;
    start = clock();
    for (i = 0; i < Trials; i++) {
        key = ((int) (drand48() * range_num_ints * 2)) + 1;
        if (Verbose) printf("Trial %d, Key %d", i, key);
        if (drand48() < 0.5) {
            ip = (int *) malloc(sizeof(int));
            assert(ip != NULL);
            *ip = -key;
            /* insert returns 1 if key not found, 0 if found */
            if (tree_insert(test_tree, key, ip) == 1) {
                unsuc_search += tree_stats(test_tree);
                unsuc_trials++;
                keys_added++;
                if (Verbose) printf(" added\n");
            } else {
                suc_search += tree_stats(test_tree);
                suc_trials++;
                if (Verbose) printf(" replaced\n");
            }
        } else {
            dp = tree_remove(test_tree, key);
            if (dp == NULL) {
                if (Verbose) printf(" not found\n");
                unsuc_search += tree_stats(test_tree);
                unsuc_trials++;
            } else {
                if (Verbose) printf(" removed\n");
                suc_search += tree_stats(test_tree);
                suc_trials++;
                keys_removed++;
                assert(*(int *)dp == -key);
                free(dp);
            }
        }
        /* this prints the tree after each trial */
        if (Verbose)
            tree_debug_print_tree(test_tree);
    }
    end = clock();
    size += keys_added - keys_removed;
    assert(size == tree_size(test_tree));
    printf("  After exercise, time=%g ms, new tree size=%d\n",
            1000*((double)(end-start))/CLOCKS_PER_SEC, size);
    printf("  Successful lookups during insert/remove exercise=");
    if (suc_trials > 0) printf("%g", (double)suc_search / suc_trials);
    else printf("N/A");
    printf(", trials=%d\n", suc_trials);
    printf("  Unsuccessful lookups during insert/remove exercise=");
    if (unsuc_trials > 0) printf("%g", (double)unsuc_search / unsuc_trials);
    else printf("N/A");
    printf(", trials=%d\n", unsuc_trials);

    printf("  Validating tree...");
    tree_debug_validate(test_tree);
    printf("passed\n");

    /* test search times for new tree */
    suc_search = unsuc_search = 0;
    suc_trials = unsuc_trials = 0;
    start = clock();
    for (i = 0; i < Trials; i++) {
        key = ((int) (drand48() * range_num_ints * 2)) + 1;
        dp = tree_search(test_tree, key);
        if (dp == NULL) {
            unsuc_search += tree_stats(test_tree);
            unsuc_trials++;
        } else {
            suc_search += tree_stats(test_tree);
            suc_trials++;
        }
    }
    end = clock();
    size = tree_size(test_tree);
    ipl = tree_internal_path_len(test_tree);
    printf("  After search experiment, time=%g ms, tree size=%d\n",
            1000*((double)(end-start))/CLOCKS_PER_SEC, size);
    print_search_summary(size, ipl, suc_search, suc_trials,
            unsuc_search, unsuc_trials, TRUE);

    /* remove and free all items from tree */
    tree_destruct(test_tree);

    printf("----- End of equilibrium test -----\n\n");
}

/* create one node to be inserted in tree with an even key
 */
void build_one_node(tree_t *T, int key)
{
    int *np = (int *) malloc(sizeof(int));
    assert(np != NULL);
    *np = -2*key;
    tree_insert(T, 2*key, np);
}

/* build a complete tree with the lowest level full.  The tree has 
 * 2^(levels)-1 nodes and is perfectly balanced.  The key range is twice the 
 * size of the tree and only the even keys are inserted in the tree.
 */
void build_optimal(tree_t *T, int levels)
{
    int n, start, inc, i, j;
    n = pow(2, levels);
    for (i = 0; i<levels; i++) {
        start = n/pow(2, i + 1); inc = 2 * start;
        for (j = start; j < n; j += inc) {
            build_one_node(T, j);
        }
    }
}
/* build a random tree with 2^(levels)-1 nodes.  The nodes are numbered 2 to 2n and are
 * inserted randomly with a uniform distribution.  Only the even keys are
 * inserted.  Use Knuth shuffle to create random permutation.
 */
void build_random(tree_t *T, int levels)
{
    int nodes = pow(2,levels)-1;
    int *narray;
    int i, key, temp;
    narray = (int *) malloc((size_t)nodes * sizeof(int));
    assert(nodes == 0 || narray != NULL);
    for (i = 0; i<nodes; i++)
        narray[i] = i;
    for (i = 0; i<nodes; i++) {
        key = (int) (drand48() * (nodes - i)) + i;
        assert(i <= key && key < nodes);
        temp = narray[i]; narray[i] = narray[key]; narray[key] = temp;
        build_one_node(T, narray[i]+1);
    }
    free(narray);
}

/* build a tree with 2^(levels)-1 nodes.  The nodes are numbered 2 to 2n and are
 * inserted in a poor order.  Only the even keys are inserted.  
 * 
 * The number of sets is sets
 * The size of a set is set_size
 *
 * Keys in a set are ascending, descending, or zig-zag
 */
void build_poor(tree_t *T, int levels)
{
    int n, start, inc, i, j;
    int low, high;
    n = pow(2, levels);
    for (i = 0; i<levels/2; i++) {
        start = n/pow(2, i + 1); inc = 2 * start;
        for (j = start; j < n; j += inc) {
            build_one_node(T, j);
        }
    }
    int sets = pow(2, levels/2);
    int set_size = pow(2, levels - levels/2);
    for (i = 0; i < sets; i++) {
        low = i*set_size + 1; high = (i+1)*set_size - 1;
        int type = (int)(4 * drand48());
        // randomly select type of insertion pattern
        if (type == 0) {  // ascending
            for (j = low; j <= high; j++) {
                build_one_node(T, j);
            }
        } else if (type == 1) {  // descending
            for (j = high; j >= low; j--) {
                build_one_node(T, j);
            }
        } else if (type == 2) {  // zig-zag, low first
            for (j = 0; j < (set_size-1)/2; j++) {
                build_one_node(T, low + j);
                build_one_node(T, high - j);
            }
            if (j*2 != set_size-1) {
                build_one_node(T, low + j);
            }
        } else {  // zig-zag, high first
            for (j = 0; j < (set_size-1)/2; j++) {
                build_one_node(T, high - j);
                build_one_node(T, low + j);
            }
            if (j*2 != set_size-1) {
                build_one_node(T, low + j);
            }
        }
    }
}

static void print_usage(FILE *stream, const char *program)
{
    fprintf(stream,
            "Usage: %s [drivers] [options]\n"
            "Drivers (select at least one):\n"
            "  -o          search in an optimal tree\n"
            "  -r          search in a random tree\n"
            "  -p          search in a tree with poor insertion order\n"
            "  -e          random insert/remove equilibrium test\n"
            "  -u number   unit driver (provided: 0 through 8)\n"
            "Options:\n"
            "  -w levels   initial size is 2^levels - 1 (0..20; default 16)\n"
            "              unit 7 requires -w 0..4\n"
            "  -t trials   trials per phase (0..INT_MAX; default 100000)\n"
            "  -s seed     random seed (0..INT_MAX; default 11082026)\n"
            "  -v          verbose output (use small -w and -t)\n"
            "  -h, --help  show this help and exit\n",
            program);
}

static int parse_number(const char *text, int minimum, int maximum, char option)
{
    char *end;
    long value;
    errno = 0;
    value = strtol(text, &end, 10);
    if (errno == ERANGE || text == end || *end != '\0' ||
            value < minimum || value > maximum) {
        fprintf(stderr, "Invalid -%c value '%s': expected an integer in [%d, %d].\n",
                option, text, minimum, maximum);
        exit(EXIT_FAILURE);
    }
    return (int)value;
}

/* Read POSIX command-line options.  Students may add new -u cases in main. */
void getCommandLine(int argc, char **argv)
{
    int c;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_usage(stdout, argv[0]);
            exit(EXIT_SUCCESS);
        }
    }
    opterr = 0;
    while ((c = getopt(argc, argv, "w:t:s:veoru:ph")) != -1) {
        switch (c) {
            case 'w': Levels = parse_number(optarg, 0, 20, c); break;
            case 't': Trials = parse_number(optarg, 0, INT_MAX, c); break;
            case 's': Seed = parse_number(optarg, 0, INT_MAX, c); break;
            case 'v': Verbose = TRUE; break;
            case 'e': EquilibriumTest = TRUE; break;
            case 'o': OptimalTest = TRUE; break;
            case 'r': RandomTest = TRUE; break;
            case 'p': PoorTest = TRUE; break;
            case 'u': UnitNumber = parse_number(optarg, 0, INT_MAX, c); break;
            case 'h': print_usage(stdout, argv[0]); exit(EXIT_SUCCESS);
            default:
                fprintf(stderr, "Unknown option or missing argument. Use -h for help.\n");
                exit(EXIT_FAILURE);
        }
    }
    if (optind != argc) {
        fprintf(stderr, "Unexpected argument '%s'. Use ASCII '-' for options.\n", argv[optind]);
        exit(EXIT_FAILURE);
    }
    if (!(OptimalTest || RandomTest || PoorTest || EquilibriumTest || UnitNumber >= 0)) {
        print_usage(stderr, argv[0]);
        exit(EXIT_FAILURE);
    }
}

/* Additional deletion cases. Students must add their own boundary and
 * correctness tests as described in the assignment.
 */
void big_test(void)
{
    printf("case 1.  No children  (three sub cases, parent on left, right, or root)\n");

    const int ins_keys1[] = {4, 2, 6};
    const int del_keys1[] = {2, 6, 4}; 
    unitDriver(ins_keys1, sizeof ins_keys1 / sizeof(int), del_keys1, sizeof del_keys1 / sizeof(int));

    printf("\ncase 2: node 10 has only one child on left\n");

    printf("\tleft of parent\n");
    const int ins_keys2l[] = {20, 10, 5, 3, 7};
    const int del_keys2l[] = {10}; 
    unitDriver(ins_keys2l, sizeof ins_keys2l / sizeof(int), del_keys2l, sizeof del_keys2l / sizeof(int));

    printf("\tright of parent\n");
    const int ins_keys2r[] = {2, 10, 5, 3, 7};
    const int del_keys2r[] = {10}; 
    unitDriver(ins_keys2r, sizeof ins_keys2r / sizeof(int), del_keys2r, sizeof del_keys2r / sizeof(int));

    printf("\troot\n");
    const int ins_keys2[] = {10, 5, 3, 7};
    const int del_keys2[] = {10}; 
    unitDriver(ins_keys2, sizeof ins_keys2 / sizeof(int), del_keys2, sizeof del_keys2 / sizeof(int));

    printf("\nvariation 1:  append 6 to ins_keys\n");
    printf("\tleft of parent\n");
    const int ins_keys2l_1[] = {20, 10, 5, 3, 7, 6};
    const int del_keys2l_1[] = {10}; 
    unitDriver(ins_keys2l_1, sizeof ins_keys2l_1 / sizeof(int), del_keys2l_1, sizeof del_keys2l_1 / sizeof(int));

    printf("\tright of parent\n");
    const int ins_keys2r_1[] = {2, 10, 5, 3, 7, 6};
    const int del_keys2r_1[] = {10}; 
    unitDriver(ins_keys2r_1, sizeof ins_keys2r_1 / sizeof(int), del_keys2r_1, sizeof del_keys2r_1 / sizeof(int));

    printf("\troot\n");
    const int ins_keys2_1[] = {10, 5, 3, 7, 6};
    const int del_keys2_1[] = {10}; 
    unitDriver(ins_keys2_1, sizeof ins_keys2_1 / sizeof(int), del_keys2_1, sizeof del_keys2_1 / sizeof(int));

    //variation 2:  append 9, 8
    printf("\nvariation 2:  append 9, 8 to ins_keys\n");
    printf("\tleft of parent\n");
    const int ins_keys2l_2[] = {20, 10, 5, 3, 7, 6, 9, 8};
    const int del_keys2l_2[] = {10}; 
    unitDriver(ins_keys2l_2, sizeof ins_keys2l_2 / sizeof(int), del_keys2l_2, sizeof del_keys2l_2 / sizeof(int));

    printf("\tright of parent\n");
    const int ins_keys2r_2[] = {2, 10, 5, 3, 7, 6, 9, 8};
    const int del_keys2r_2[] = {10}; 
    unitDriver(ins_keys2r_2, sizeof ins_keys2r_2 / sizeof(int), del_keys2r_2, sizeof del_keys2r_2 / sizeof(int));

    printf("\troot\n");
    const int ins_keys2_2[] = {10, 5, 3, 7, 6, 9, 8};
    const int del_keys2_2[] = {10}; 
    unitDriver(ins_keys2_2, sizeof ins_keys2_2 / sizeof(int), del_keys2_2, sizeof del_keys2_2 / sizeof(int));

    printf("\ncase 3:  node 10 has only one child on right\n");
    printf("\tleft of parent\n");
    const int ins_keys3l[] = {20, 10, 15, 13, 17};
    const int del_keys3l[] = {10}; 
    unitDriver(ins_keys3l, sizeof ins_keys3l / sizeof(int), del_keys3l, sizeof del_keys3l / sizeof(int));

    printf("\tright of parent\n");
    const int ins_keys3r[] = {2, 10, 15, 13, 17};
    const int del_keys3r[] = {10}; 
    unitDriver(ins_keys3r, sizeof ins_keys3r / sizeof(int), del_keys3r, sizeof del_keys3r / sizeof(int));

    printf("\troot\n");
    const int ins_keys3[] = {10, 15, 13, 17};
    const int del_keys3[] = {10}; 
    unitDriver(ins_keys3, sizeof ins_keys3 / sizeof(int), del_keys3, sizeof del_keys3 / sizeof(int));

    printf("\nvariation 1:  append 14\n");
    printf("\tleft of parent\n");
    const int ins_keys3l_1[] = {20, 10, 15, 13, 17, 14};
    const int del_keys3l_1[] = {10}; 
    unitDriver(ins_keys3l_1, sizeof ins_keys3l_1 / sizeof(int), del_keys3l_1, sizeof del_keys3l_1 / sizeof(int));

    printf("\tright of parent\n");
    const int ins_keys3r_1[] = {2, 10, 15, 13, 17, 14};
    const int del_keys3r_1[] = {10}; 
    unitDriver(ins_keys3r_1, sizeof ins_keys3r_1 / sizeof(int), del_keys3r_1, sizeof del_keys3r_1 / sizeof(int));

    printf("\troot\n");
    const int ins_keys3_1[] = {10, 15, 13, 17, 14};
    const int del_keys3_1[] = {10}; 
    unitDriver(ins_keys3_1, sizeof ins_keys3_1 / sizeof(int), del_keys3_1, sizeof del_keys3_1 / sizeof(int));

    //variation 2:  append 11, 12
    printf("\nvariation 2:  append 14, 11, 12\n");
    printf("\tleft of parent\n");
    const int ins_keys3l_2[] = {20, 10, 15, 13, 17, 14, 11, 12};
    const int del_keys3l_2[] = {10}; 
    unitDriver(ins_keys3l_2, sizeof ins_keys3l_2 / sizeof(int), del_keys3l_2, sizeof del_keys3l_2 / sizeof(int));

    printf("\tright of parent\n");
    const int ins_keys3r_2[] = {2, 10, 15, 13, 17, 14, 11, 12};
    const int del_keys3r_2[] = {10}; 
    unitDriver(ins_keys3r_2, sizeof ins_keys3r_2 / sizeof(int), del_keys3r_2, sizeof del_keys3r_2 / sizeof(int));

    printf("\troot\n");
    const int ins_keys3_2[] = {10, 15, 13, 17, 14, 11, 12};
    const int del_keys3_2[] = {10}; 
    unitDriver(ins_keys3_2, sizeof ins_keys3_2 / sizeof(int), del_keys3_2, sizeof del_keys3_2 / sizeof(int));

    printf("\ncase 4:  node 10 has two children but no left-right child\n");
    printf("\tleft of parent\n");
    const int ins_keys4l[] = {20, 10, 5, 3, 15, 13, 17};
    const int del_keys4l[] = {10}; 
    unitDriver(ins_keys4l, sizeof ins_keys4l / sizeof(int), del_keys4l, sizeof del_keys4l / sizeof(int));

    printf("\tright of parent\n");
    const int ins_keys4r[] = {2, 10, 5, 3, 15, 13, 17};
    const int del_keys4r[] = {10}; 
    unitDriver(ins_keys4r, sizeof ins_keys4r / sizeof(int), del_keys4r, sizeof del_keys4r / sizeof(int));

    printf("\troot\n");
    const int ins_keys4[] = {10, 5, 3, 15, 13, 17};
    const int del_keys4[] = {10}; 
    unitDriver(ins_keys4, sizeof ins_keys4 / sizeof(int), del_keys4, sizeof del_keys4 / sizeof(int));

    printf("\tvariation:  append 11, 12, 14\n");
    printf("\tleft of parent\n");
    const int ins_keys4l_1[] = {20, 10, 5, 3, 15, 13, 17, 11, 12, 14};
    const int del_keys4l_1[] = {10}; 
    unitDriver(ins_keys4l_1, sizeof ins_keys4l_1 / sizeof(int), del_keys4l_1, sizeof del_keys4l_1 / sizeof(int));

    printf("\tright of parent\n");
    const int ins_keys4r_1[] = {2, 10, 5, 3, 15, 13, 17, 11, 12, 14};
    const int del_keys4r_1[] = {10}; 
    unitDriver(ins_keys4r_1, sizeof ins_keys4r_1 / sizeof(int), del_keys4r_1, sizeof del_keys4r_1 / sizeof(int));

    printf("\troot\n");
    const int ins_keys4_1[] = {10, 5, 3, 15, 13, 17, 11, 12, 14};
    const int del_keys4_1[] = {10}; 
    unitDriver(ins_keys4_1, sizeof ins_keys4_1 / sizeof(int), del_keys4_1, sizeof del_keys4_1 / sizeof(int));

    printf("\ncase 5:  node 10 has two children but no right-left child\n");
    printf("\tleft of parent\n");
    const int ins_keys5l[] = {20, 10, 5, 3, 15, 7, 17, 6};
    const int del_keys5l[] = {10}; 
    unitDriver(ins_keys5l, sizeof ins_keys5l / sizeof(int), del_keys5l, sizeof del_keys5l / sizeof(int));

    printf("\tright of parent\n");
    const int ins_keys5r[] = {2, 10, 5, 3, 15, 7, 17, 6};
    const int del_keys5r[] = {10}; 
    unitDriver(ins_keys5r, sizeof ins_keys5r / sizeof(int), del_keys5r, sizeof del_keys5r / sizeof(int));

    printf("\troot\n");
    const int ins_keys5[] = {10, 5, 3, 15, 7, 17, 6};
    const int del_keys5[] = {10}; 
    unitDriver(ins_keys5, sizeof ins_keys5 / sizeof(int), del_keys5, sizeof del_keys5 / sizeof(int));

    printf("\tvariation: append 9, 8\n");
    printf("\tleft of parent\n");
    const int ins_keys5l_1[] = {20, 10, 5, 3, 15, 7, 17, 6, 9, 8};
    const int del_keys5l_1[] = {10}; 
    unitDriver(ins_keys5l_1, sizeof ins_keys5l_1 / sizeof(int), del_keys5l_1, sizeof del_keys5l_1 / sizeof(int));

    printf("\tright of parent\n");
    const int ins_keys5r_1[] = {2, 10, 5, 3, 15, 7, 17, 6, 9, 8};
    const int del_keys5r_1[] = {10}; 
    unitDriver(ins_keys5r_1, sizeof ins_keys5r_1 / sizeof(int), del_keys5r_1, sizeof del_keys5r_1 / sizeof(int));

    printf("\troot\n");
    const int ins_keys5_1[] = {10, 5, 3, 15, 7, 17, 6, 9, 8};
    const int del_keys5_1[] = {10}; 
    unitDriver(ins_keys5_1, sizeof ins_keys5_1 / sizeof(int), del_keys5_1, sizeof del_keys5_1 / sizeof(int));
    
    printf("\ncase 6:  node 100 has predecessor and successor that are farther away\n");

    printf("\tleft of parent\n");
    const int ins_keys6l[] = {200, 100, 50, 150, 25, 75, 90, 80, 85, 140, 110, 130, 120};
    const int del_keys6l[] = {100, 90}; 
    unitDriver(ins_keys6l, sizeof ins_keys6l / sizeof(int), del_keys6l, sizeof del_keys6l / sizeof(int));

    printf("\tright of parent\n");
    const int ins_keys6r[] = {2, 100, 50, 150, 25, 75, 90, 80, 85, 140, 110, 130, 120};
    const int del_keys6r[] = {100, 110}; 
    unitDriver(ins_keys6r, sizeof ins_keys6r / sizeof(int), del_keys6r, sizeof del_keys6r / sizeof(int));

    printf("\troot\n");
    const int ins_keys6[] = {100, 50, 150, 25, 75, 90, 80, 85, 140, 110, 130, 120};
    const int del_keys6[] = {100, 90, 110}; 
    unitDriver(ins_keys6, sizeof ins_keys6 / sizeof(int), del_keys6, sizeof del_keys6 / sizeof(int));
}

/* vi:set ts=8 sts=4 sw=4 et: */
