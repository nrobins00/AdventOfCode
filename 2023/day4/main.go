package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"os"
	"strings"
)

func main() {
	f, err := os.Open("./input.txt")
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()

	scanner := bufio.NewScanner(f)

	cardCounts := make(map[int]int)

	cardNum := 1
	sum := 0
	for scanner.Scan() {
		rawLine := scanner.Text()
		colonSplit := strings.Split(rawLine, ":")
		if len(colonSplit) < 2 {
			break
		}
		cardCounts[cardNum] = cardCounts[cardNum] + 1
		game := colonSplit[1]
		pipeSplit := strings.Split(game, "|")
		winningNums := strings.Split(pipeSplit[0], " ")
		myNums := strings.Split(pipeSplit[1], " ")

		numMatches := 0
		for _, myNum := range myNums {
			if len(strings.TrimSpace(myNum)) == 0 {
				continue
			}
			for _, winningNum := range winningNums {
				if myNum == winningNum {
					numMatches = numMatches + 1
				}
			}
		}
		if cardNum == 3 {
			fmt.Println(cardCounts[3])
		}
		for cardOffset := 1; cardOffset <= numMatches; cardOffset++ {
			cardCounts[cardNum+cardOffset] = cardCounts[cardNum+cardOffset] + 1*cardCounts[cardNum]
		}
		sum = sum + int(math.Pow(2, float64(numMatches-1)))
		fmt.Println(sum)
		cardNum = cardNum + 1
	}
	cardCountSum := 0
	for _, val := range cardCounts {
		cardCountSum = cardCountSum + val
	}
	fmt.Println(cardCountSum)
	//fmt.Println(sum)
}
