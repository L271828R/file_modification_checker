#!/bin/bash

# Compile the main source code
gcc -c -o ./src/latest.o ./src/latest.c

# Compile the test source file along with the main source
gcc -o ./tests/test_is_ignored -DTESTING ./tests/test_is_ignored.c ./src/latest.o

# Run the compiled test
./tests/test_is_ignored


