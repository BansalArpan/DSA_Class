# MP5 Binary Search Trees

ECE 2230, Fall 2026. See `mp5.pdf` on Canvas for the complete requirements.

## Getting started

Download `mp5-main.zip` from the MP5 assignment on Canvas and extract it.
The archive contains `lab5.c` (the provided test drivers), this README, and
`.gitignore`.

You must create your own BST source file, header file, and makefile. These files
are intentionally not included in the starter. The driver includes `tree.h`;
if you use another header name, update that include. Implement the interfaces
specified in the PDF, add your own unit drivers to `lab5.c`, and prepare your
own test script, test plan, and performance evaluation.

Use a Linux or other POSIX environment with a C compiler and `make`. The driver
uses POSIX functions including `getopt`, `drand48`, and `srand48`. A native
Windows compiler may require a Linux environment such as WSL.

After you have implemented your module, this example builds it (adjust the
source filename if necessary):

```sh
cc -std=gnu11 -Wall -Wextra -g lab5.c tree.c -lm -o lab5
./lab5 --help
./lab5 -o -w 3 -t 10 -v
./lab5 -u 5
./lab5 -u 7 -w 3
```

Your makefile must build the executable `lab5`. The commands above are examples;
you must still write and submit your own makefile and test script. Use the
ASCII hyphen `-` when entering command-line options.

## Provided drivers

- `-o`, `-r`, and `-p`: build an optimal, random, or poor-shaped BST and test
  searches. `-w levels` sets the initial size to `2^levels - 1` nodes.
- `-u number`: run a unit driver. Cases 0 through 8 are provided; add cases for
  your own tests. Case 5 checks deletion of a missing key and should report
  that returning `NULL` is correct. Case 7 requires `-w` from 0 through 4.
- `-e`: build a random BST, then perform random insertions and removals,
  followed by searches in the resulting tree.

Defaults are `-w 16`, `-t 100000`, and `-s 11082026`. Supported levels are
0 through 20. Trials and seeds must be nonnegative integers no larger than
`INT_MAX`. Use `-h` or `--help` for the option list. Select at least one driver.
Use small trees and trial counts with `-v`.

The driver prints `N/A` when an average is undefined because there are no
observations of that kind or no keys in the tree. The equilibrium
unsuccessful-search values are labeled uniform-gap benchmarks; read the PDF's
performance evaluation section before comparing it with the measured value.

On Linux, use Valgrind to check memory errors and leaks as described in the
PDF. Valgrind is an external tool, not a file supplied in this starter.

## Submission

Upload one ZIP file to the MP5 assignment on Canvas. Include:

- `lab5.c`, including your additional unit test drivers.
- Your BST source and header files (for example, `tree.c` and `tree.h`).
- `makefile` that builds `lab5`.
- Your test plan, including your test script and relevant program output.
- Your performance evaluation.

Do not include compiled executables, object files, temporary files, or a
`.git` directory. See Canvas for the due date. You can submit more than once;
only the final submission will be graded.

If updated starter files are posted on Canvas, incorporate the updates while
preserving your own work. Follow the course syllabus and programming policies.
