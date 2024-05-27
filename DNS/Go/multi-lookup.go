package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"sync"
	"time"
)

const SBUF_SIZE = 1025
const MinArgs = 2
const Usage = "<input_files> <output_file>"
const INPUTFS = "%1024s"
const INET6_ADDRSTRLEN=46

func resolver(hostnames <-chan string, results chan<- string, wg *sync.WaitGroup) {
	defer wg.Done()
	for hostname := range hostnames {
		addrs, err := net.LookupIP(hostname)
		if err != nil {
			results <- fmt.Sprintf("%s,%s", hostname, err)
			continue
		}
			results <- fmt.Sprintf("%s,%s", hostname, addrs[0].String())
	}
}

func requester(files []string, hostnames chan<- string, wg *sync.WaitGroup) {
	defer wg.Done()
	for _, file := range files {
		f, err := os.Open(file)
		if err != nil {
			fmt.Fprintf(os.Stderr, "Error Opening Input File: %s\n", file)
			continue
		}
		scanner := bufio.NewScanner(f)
		for scanner.Scan() {
			hostnames <- scanner.Text()
		}
		f.Close()
		if err := scanner.Err(); err != nil {
			fmt.Fprintf(os.Stderr, "Error Reading Input File: %s\n", file)
		}
	}
}

func main() {
	if len(os.Args) < MinArgs+1 {
		fmt.Fprintf(os.Stderr, "Not enough arguments: %d\n", len(os.Args)-1)
		fmt.Fprintf(os.Stderr, "Usage:\n %s %s\n", os.Args[0], Usage)
		os.Exit(1)
	}


	inputFiles := os.Args[1 : len(os.Args)-1]
	outputFile := os.Args[len(os.Args)-1]

	hostnames := make(chan string, 10)
	results := make(chan string, 10)

	var requesterWG sync.WaitGroup
	var resolverWG sync.WaitGroup

	start_time := time.Now()

	// Start requester goroutines
	requesterWG.Add(1)
	go requester(inputFiles, hostnames, &requesterWG)

	// Start resolver goroutines
	numResolvers := 2
	for i := 0; i < numResolvers; i++ {
		resolverWG.Add(1)
		go resolver(hostnames, results, &resolverWG)
	}

	// Close hostnames channel when all requesters are done
	go func() {
		requesterWG.Wait()
		close(hostnames)
	}()

	go func() {
		resolverWG.Wait()
		close(results)
	}()

	// Open the output file
	outputFP, err := os.Create(outputFile)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error Opening Output File: %v\n", err)
		os.Exit(1)
	}
	defer outputFP.Close()

	// Write results to the output file
	for result := range results {
		fmt.Fprintln(outputFP, result)
	}
	
	elapsed := time.Since(start_time)


	fmt.Println("All threads were completed!")


	fmt.Println(elapsed.Seconds())
} 
