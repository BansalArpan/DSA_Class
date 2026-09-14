# The makefile for MP1.

lab1 : ids.c ids.h lab1.c
	gcc -Wall -g ids.c lab1.c -o lab1

#  @ prefix suppresses output of shell command
#  - prefix ignore errors
#  @command || true to avoid Make's error
#  : is shorthand for true
#
#  If "make design" has any output, you will lose lots of points!
design :
	@grep "db_size" lab1.c ||:
	@grep "db_entries" lab1.c ||:
	@grep -e "-> *alert_ptr" lab1.c ||:

clean :
	rm -f *.o lab1 core gradingout*

