#!/bin/csh

#number of threads:
foreach t ( 1 2 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50)
	gcc -DNUMT=$t proj.c -o proj -lm -fopenmp -std=c99
	./proj
end
