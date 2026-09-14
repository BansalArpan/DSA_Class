#!/usr/bin/sh
#
# if this script is not executable then do
#     chmod +x valrun.sh
#
# MP1 place in student directory
valgrind --leak-check=yes ./lab1 2 < tests/t00testinput_2 > gradingout_t00testinput_2 2> gradingout_val_t00testinput_2
valgrind --leak-check=yes ./lab1 10 < tests/t01quit_10 > gradingout_t01quit_10 2> gradingout_val_t01quit_10
valgrind --leak-check=yes ./lab1 2 < tests/t02onereq_2 > gradingout_t02onereq_2 2> gradingout_val_t02onereq_2
valgrind --leak-check=yes ./lab1 2 < tests/t03oneadd_2 > gradingout_t03oneadd_2 2> gradingout_val_t03oneadd_2
valgrind --leak-check=yes ./lab1 4 < tests/t04oneaddremove_4 > gradingout_t04oneaddremove_4 2> gradingout_val_t04oneaddremove_4
valgrind --leak-check=yes ./lab1 4 < tests/t05oneaddrevip_4 > gradingout_t05oneaddrevip_4 2> gradingout_val_t05oneaddrevip_4
valgrind --leak-check=yes ./lab1 9 < tests/t06nineadds_9 > gradingout_t06nineadds_9 2> gradingout_val_t06nineadds_9
valgrind --leak-check=yes ./lab1 2 < tests/t07addsrealloc_2 > gradingout_t07addsrealloc_2 2> gradingout_val_t07addsrealloc_2
valgrind --leak-check=yes ./lab1 4 < tests/t08addremove_4 > gradingout_t08addremove_4 2> gradingout_val_t08addremove_4
valgrind --leak-check=yes ./lab1 2 < tests/t09duplicates_2 > gradingout_t09duplicates_2 2> gradingout_val_t09duplicates_2
valgrind --leak-check=yes ./lab1 2 < tests/t10onescan_2 > gradingout_t10onescan_2 2> gradingout_val_t10onescan_2
valgrind --leak-check=yes ./lab1 2 < tests/t11scans_2 > gradingout_t11scans_2 2> gradingout_val_t11scans_2
valgrind --leak-check=yes ./lab1 8 < tests/t12dels_8 > gradingout_t12dels_8 2> gradingout_val_t12dels_8
valgrind --leak-check=yes ./lab1 5 < tests/t13nomatches_5 > gradingout_t13nomatches_5 2> gradingout_val_t13nomatches_5
valgrind --leak-check=yes ./lab1 10000 < tests/t14emptylist_10000 > gradingout_t14emptylist_10000 2> gradingout_val_t14emptylist_10000

