package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

func main() {
	data := parseInput("input")
	times := strings.Split(data[0], " ")
	distances := strings.Split(data[1], " ")

	times = times[1:]
	distances = distances[1:]
	var newTimes []string
	for _, val := range times {
		_, err := strconv.Atoi(val)
		if err != nil {
			continue
		}
		newTimes = append(newTimes, val)

	}

	singleTimeString := strings.Join(newTimes, "")
	singleTimeNum, _ := strconv.Atoi(singleTimeString)

	var newDistances []string
	for _, val := range distances {
		_, err := strconv.Atoi(val)
		if err != nil {
			continue
		}
		newDistances = append(newDistances, val)
	}

	singleDistanceString := strings.Join(newDistances, "")
	singleDistanceNum, _ := strconv.Atoi(singleDistanceString)
	//---------------start
	//totalProduct := 1
	// for ix, time := range newTimes {
	waysToWin := 0
	// 	associatedDistance := distanceNums[ix]

	for holdLength := 0; holdLength < singleTimeNum; holdLength++ {
		timeTraveling := singleTimeNum - holdLength
		distanceTraveled := timeTraveling * holdLength
		if distanceTraveled > singleDistanceNum {
			waysToWin++
		}
	}
	//totalProduct *= waysToWin

	// }

	//fmt.Println(totalProduct)
	fmt.Println(waysToWin)

}

func parseInput(fileName string) []string {
	buf, err := os.Open(fileName)
	if err != nil {
		log.Fatal(err)
	}
	scanner := bufio.NewScanner(buf)

	var ret []string
	for scanner.Scan() {
		line := scanner.Text()
		if line != "" {
			ret = append(ret, line)
		}
	}
	return ret
}
