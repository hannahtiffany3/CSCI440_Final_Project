# CSCI440_Final_Project
Researching the performance of multithreaded programs in C and Go

# Matrix Multiplication:

MM implementation in Go:

https://onyalioscar.medium.com/concurrent-matrix-multiplication-in-golang-eaa98663ebe6

To build and run: go run MM.go {size} {maximum}

Size: size of the two matrices (they will both be “size x size” matrices) 

Maximum: highest number to be generated

MM implementation in C:

To build: gcc MM.c -o MM

To run: ./MM {size} {cutoff}

Size: size of the two matrices (they will both be “size x size” matrices) 

Maximum: the highest number to be generated (max size is 5000)

# Monte Carlo Pi:

Implementation in C:

https://stackoverflow.com/questions/61951442/implement-program-monte-carlo-calculation-pi-use-serial-and-multithread

To build: gcc MCP.c -o MCP -lm

To run: ./MCP {number}

Number: number of points used to calculate pi

Implementation in Go:

To build/run: go run MCP.go {number}

Number: number of points used to calculate pi

Test values: 1,000,000, 5,000,000, 10,000,000

# DNS:

Implementation in C:

Complied with make

./multi_lookup {input file(s)} {output file}

Implementation in GO:

go run multi-lookup.go {input files(s)} {output file}

# Python Test Scripts

1. Run test 3 times
2. Calculate
3. Calculate standard deviation
4. Calculate error
5. Repeat steps 2-4 until error<.1*mean


