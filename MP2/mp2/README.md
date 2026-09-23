# MP2: Linked Lists

See the MP2 assignment document on Canvas for the full requirements.

## Getting started

1. Download the MP2 starter ZIP from Canvas and extract it.
2. Work inside the `mp2` directory.
3. Update the file-header comments in your source files with your name and Clemson username.
4. Build the program with:

   ```bash
   make
   ```

For Part 1, use the provided `driver.c` to test the linked-list ADT. Build it with:

```bash
make driver
./driver
```

You should extend `driver.c` with your own tests as you develop your list functions.

To test the complete program against the provided example:

```bash
./lab2 < testinput > myoutput
diff -w myoutput expectedoutput
```

Also run:

```bash
make design
```

to check the list-ADT encapsulation requirements described in the assignment.

## Canvas submission

Submit **one ZIP file** to the MP2 assignment in Canvas. Put the following files at the top level of your submission ZIP:

- `lab2.c`
- `ids_support.c`
- `llist.c`
- `datatypes.h`
- `ids_support.h`
- `llist.h`
- `makefile`
- one input test file that you created (for example, `mytestinput`)

Do **not** include `driver.c`, executables, object files, or output files in your submission ZIP.

You may submit more than once in Canvas. Only the most recent submission received before the Canvas deadline will be graded.
