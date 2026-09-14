#!/usr/bin/sh
#
# if this script is not executable then do
#     chmod +x run.sh
#
# this script assumes that lab1 is in the current directory
# and there is a tests directory with the input files
#
# MP1 place in student directory
./lab1 2 < tests/t00testinput_2 > gradingout_t00testinput_2
./lab1 10 < tests/t01quit_10 > gradingout_t01quit_10
./lab1 2 < tests/t02onereq_2 > gradingout_t02onereq_2
./lab1 2 < tests/t03oneadd_2 > gradingout_t03oneadd_2
./lab1 4 < tests/t04oneaddremove_4 > gradingout_t04oneaddremove_4
./lab1 4 < tests/t05oneaddrevip_4 > gradingout_t05oneaddrevip_4
./lab1 9 < tests/t06nineadds_9 > gradingout_t06nineadds_9
./lab1 2 < tests/t07addsrealloc_2 > gradingout_t07addsrealloc_2
./lab1 4 < tests/t08addremove_4 > gradingout_t08addremove_4
./lab1 2 < tests/t09duplicates_2 > gradingout_t09duplicates_2
./lab1 2 < tests/t10onescan_2 > gradingout_t10onescan_2
./lab1 2 < tests/t11scans_2 > gradingout_t11scans_2
./lab1 8 < tests/t12dels_8 > gradingout_t12dels_8
./lab1 5 < tests/t13nomatches_5 > gradingout_t13nomatches_5
./lab1 10000 < tests/t14emptylist_10000 > gradingout_t14emptylist_10000
