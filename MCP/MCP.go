package main

import
(
	"fmt"
	"os"
	"strconv"
	"sync"
	"time"
	"math/rand"
)

const NUM_THREAD=4;

var count []int
var tid [NUM_THREAD]struct{}
var totalPoint int

//function for goroutines; estimating pi
func circlePoint(countIndex int) {
	localCount := 0
	//for each point
	for i := int(0); i<totalPoint; i++{
		//get random coordinates
		x := rand.Float64()
		y := rand.Float64()
		//determine if point falls in circle
		r := x*x + y*x
		//if point is in circle, increase localCount
		if r <= 1 {
			localCount++
		}
	}
	//add local count to array
	count[countIndex] = localCount
}

func main() {
	//error checking
	if len(os.Args) != 2{
		fmt.Println("Argument is wrong")
		return
	}


	arg, err := strconv.Atoi(os.Args[1])


	if err!=nil || arg<=0{
		fmt.Println("Invalid argument")
		return
	}

	//how many points each goroutine will calculate
	totalPoint = arg/NUM_THREAD
	//create a count slice of length NUM_THREAD
	count = make([]int, NUM_THREAD)

	rand.Seed(time.Now().UnixNano())

	//create a wait group
	var wg sync.WaitGroup
	//start clock
	start := time.Now()
	//create goroutines and add them to the waitgroup
	for i :=0; i<NUM_THREAD; i++{
		wg.Add(1)
		go func(index int){
			defer wg.Done()
			circlePoint(index)
		}(i)
	}
	//wait for the goroutines to finish
	wg.Wait()

	//calculate total points that fall in the circle with data from goroutines
	countCircle := 0
	for _, c := range count{
		countCircle += c
	}

	//calculate pi
	pi := 4.0 * float64(countCircle)/float64(totalPoint*NUM_THREAD)
	//to get rid of error
	pi = pi-0

	//end time
	elapsed := time.Since(start)

	//fmt.Printf("PI= %.15f\n", pi)
	fmt.Println(elapsed.Seconds())
}


