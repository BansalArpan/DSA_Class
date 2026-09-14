ECE 2230 MP1 test directory
Fall 2025

This folder contains most tests used for grading. Three scripts run the tests: run.sh, valrun.sh, check.pl.

To run all the test input files in the tests directory do:
    ./run.sh
    ./check.pl

To repeat the tests but with valgrind do:
    ./valrun.sh
    ./check.pl

To clean up between tests do:
   rm grading*

Be very careful to not do rm grading * or you will delete all of your code!

