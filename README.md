# CSCI440_Final_Project
Researching the performance of multithreaded programs in C and Go

Matrix Multiplication:

MM implementation in Go:
https://onyalioscar.medium.com/concurrent-matrix-multiplication-in-golang-eaa98663ebe6
To build and run: go run MM.go {size} {maximum}
Size: size of the two matrices (they will both be “size x size” matrices) 
Maximum: highest number to be generated

MM C implementation:
To build: gcc MM.c -o MM
To run: ./MM {size} {cutoff}
Size: size of the two matrices (they will both be “size x size” matrices) 
Maximum: the highest number to be generated (max size is 5000)

Sample input files created by gennum.go. To create the files, I ran the code then directed the output to the files. The size of the matrix and range of numbers is hard-coded in the file.
Description of files generated: 8 files of matrices of size 10, 100, and 500; numbers 1-4 contain numbers from 0-99, numbers 5-8 contain numbers from 0-9. There are also 2 small test files provided, to ensure the code works properly. 

Monte Carlo Pi:

Implementation in C:
https://stackoverflow.com/questions/61951442/implement-program-monte-carlo-calculation-pi-use-serial-and-multithread
To build: gcc MCP.c -o MCP -lm
To run: ./MCP {number}
Number: number of points used to calculate pi
Implementation in Go:
To build/run: go run MCP.go {number}
Number: number of points used to calculate pi

Test values: 1,000,000, 5,000,000, 10,000,000

DNS:

Implementation in C:
Complied with make

./multi_lookup {input file(s)} {output file}

Implementation in GO:

go run multi-lookup.go {input files(s)} {output file}