package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
	"math/rand"
	"time"
)

func concurrentMatrixAlgorithm( a[][]int, b [][]int) [][]int {
	lenA := len(a)
	colB := len(b[0])
	finalResult := make([][]int, lenA)
	rowChannel := make(chan RowResult, lenA)

	for i:=0; i<lenA; i++ {
		row := make([]int, colB)
		go populateRows(rowChannel, row, a[i], b, i, colB)
	}

	for x:=0; x<lenA; x++ {
		select {
		case rowResult:= <-rowChannel:
			position:=rowResult.index
			finalResult[position]=rowResult.result
		}
	}
	return finalResult
}

func populateRows(c chan RowResult, resultRow []int, rowA []int, matrixB [][]int, position int, colB int) {
	for i, v:=range rowA{
		for j:=0; j<colB; j++ {
			valBAtIJ:=matrixB[i][j]
			product:=v*valBAtIJ
			valueAtTempRow:=resultRow[j]
			resultRow[j]=product+valueAtTempRow
		}
	}
	c <- RowResult{result:resultRow, index: position}
}

type RowResult struct {
	result []int
	index int
}

func readMatrixFromFile(filePath string) ([][]int, error) {
	file, err := os.Open(filePath)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var matrix [][]int
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		fields := strings.Fields(line)
		row := make([]int, len(fields))
		for i, field := range fields {
			num, err := strconv.Atoi(field)
			if err != nil {
				return nil, err
			}
			row[i] = num
		}
		matrix = append(matrix, row)
	}

	if err := scanner.Err(); err != nil {
		return nil, err
	}

	return matrix, nil
}

func main(){
	args := os.Args
	if len(args)!=3{
		fmt.Println("Incorrect number of arguments")
		return
	}

	size, err:=strconv.Atoi(args[1])
	if err != nil {
		fmt.Println("Invalid size argument:", err)
		return
	}

	cutoff, err:=strconv.Atoi(args[2])
	if err != nil {
		fmt.Println("Invalid size argument:", err)
		return
	}

	matrixA := make([][]int, size)
	matrixB := make([][]int, size)
	for i:=0; i<size; i++{
		matrixA[i]=make([]int, size)
		matrixB[i]=make([]int, size)
	}

	for i:= 0; i<size; i++ {
		for j := 0; j<size; j++{
			matrixA[i][j]=rand.Intn(cutoff)
		}
	}

	for i:= 0; i<size; i++ {
		for j := 0; j<size; j++{
			matrixB[i][j]=rand.Intn(cutoff)
		}
	}

	start := time.Now()
	concurrentMatrixAlgorithm(matrixA, matrixB)
	elapsed := time.Since(start)

	// Print the result
/*	fmt.Println("Result of matrix multiplication:")
	for i:=0; i<size; i++ {
		for j:=0; j<size; j++ {
			fmt.Printf("%d ", result[i][j])
		}
		fmt.Println()
	}*/
	fmt.Println(elapsed.Seconds());
}





















