package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

type Node struct {
	label string
	left  string
	right string
}

func main() {
	data := parseInput("test.txt")

	fmt.Println(data)

}

func parseInput(fileName string) [][]int {
	buf, err := os.Open(fileName)
	if err != nil {
		log.Fatal(err)
	}
	scanner := bufio.NewScanner(buf)

	var ret [][]int
	for scanner.Scan() {
		line := scanner.Text()
		if line != "" {
			lineSlice := strings.Split(line, "")
			var lineIntSlice []int
			for _, str := range lineSlice {
				num, err := strconv.Atoi(str)
				if err != nil {
					log.Fatal(err)
				}
				lineIntSlice = append(lineIntSlice, num)
			}
			ret = append(ret, lineIntSlice)
		}
	}
	return ret
}
