#!/bin/bash

# Compile the program
gcc tlb.c -o tlb

# Execute the program with the specified inputs
./tlb $1 $2 > my_output_$1.txt