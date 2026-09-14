#!/usr/bin/perl
use strict;
use warnings;
# put in students directory and run to check against correct output file
#
# if this script is not executable then do
#     chmod +x check.pl 
#

# loop through each file and directory at current location

    # backtick executes a command and collects stdout 
    # stderr is not collected but goes to output

# @ denotes an array
my @alltest = qw(
        t00testinput_2
        t01quit_10
        t02onereq_2
        t03oneadd_2
        t04oneaddremove_4
        t05oneaddrevip_4
        t06nineadds_9
        t07addsrealloc_2
        t08addremove_4
        t09duplicates_2
        t10onescan_2
        t11scans_2
        t12dels_8
        t13nomatches_5
        t14emptylist_10000
);

my $passcount = 0;
foreach my $test (@alltest) {
    my $gradefile = 'gradingout_'.$test;
    my $valgrindfile = 'gradingout_val_'.$test;
    my $answerfile = 'out_'.$test;

    my $info = `diff -w $gradefile tests/$answerfile`;
    my $found = 0;
    if ($? > 0) {
        print "$test failed\n";
        #print "the info is: $info\n";
        $found = 1;
    } else {
        $passcount++;
    }
    if (-f $valgrindfile) {
        $info = `grep "no leaks" $valgrindfile`;
        if ($info eq "") {
            if ($found == 0) { 
                    print "$test failed valgrind\n";
            }
            $info = `grep "total heap usag" $valgrindfile`;
            print "\t$test found leaks: $info";
            $found = 1;
        }
        $info = `grep "0 errors" $valgrindfile`;
        if ($info eq "") {
            if ($found == 0) { 
                    print "$test failed valgrind\n";
            }
            $info = `grep "ERROR SUMMARY" $valgrindfile`;
            print "\t$test $info\n";
            $found = 1;
        }
    }
    if ($found == 1) {
        print "\n";
    }
}

print "Passed $passcount tests\n";
if ($passcount == 0) {
    print "\n\tWARNING no tests passed.  Make sure that testing is working\n\n";
}
    #$info = `cd $user; grep "winning amount is" gradingoutput; cd ..`;
    #print "the info is: $info\n";

    # for example, if we want to see only the errors and
    # warnings we can ignore the stdout.  To see stdout:
    # print "the info is: $info\n";
    # print "the status is $?\n";

    # system executes a command but only gets final return value
    # all output just goes to stdout
    #
    # $make = "cd $user; make clean; make; cd ..";
    # $rc = 0xffff & system $make;
    # if ($rc != 0) {
    #     die "system $make failed: $?";
    # }


#    a more elaborate way to test return value
#    $rc = 0xffff & system $make;
#    if ($rc == 0) {
#        print "ran with normal exit\n";
#    }
#    elsif ($rc == 0xff00) {
#        print "command failed: $!\n";
#    }
#    elsif (($rc & 0xff) == 0) {
#        $rc >>= 8;
#        print "ran with non-zero exit status $rc\n";
#    }
#    else {
#        print "ran with ";
#        if ($rc & 0x80) {
#            $rc &= ~0x80;
#            print "coredump from ";
#        }
#        print "signal $rc\n";
#    }

# reg expression  (upper case negates the following)
#   \d [0-9]
#   \w [a-zA-Z0-9_]
#   \s [ \r\t\n\f]
#   * zero or more
#   + one or more
#   ? zero or one

# File tests
#     -x file or directory is executable
#     -e file or directory exists
#     -z file exists and has zero size
#     -s non zero size
#     -f a file
#     -d a directory
#
