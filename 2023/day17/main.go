package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

const (
	Up    int = 1
	Right     = 2
	Down      = 3
	Left      = 4
)

type Vertex struct {
	row int
	col int
}

type Approach struct {
	direction  int //direction enum
	lineLength int //num steps in this direction
}

func main() {
	data := parseInput("test.txt")

	fmt.Println(data)

	spt := make(map[Vertex]map[Approach]int)

	startVertex := Vertex{row: 0, col: 0}
	endVertex := Vertex{len(data) - 1, len(data[0]) - 1}

	startApproach := Approach{}

	for _, prs := spt[endVertex]; !prs; _, prs = spt[endVertex] {

	}

	//fmt.Println(cost)
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
