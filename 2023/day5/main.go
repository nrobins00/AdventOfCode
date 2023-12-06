package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"os"
	"strconv"
	"strings"
)

type ConversionRange struct {
	start  int
	end    int
	offset int
}

type SeedRange struct {
	start int
	end   int
}

func main() {
	buf, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}
	scanner := bufio.NewScanner(buf)
	scanner.Scan()

	rawSeedLine := scanner.Text()
	_, rawSeedLine, _ = strings.Cut(rawSeedLine, " ")

	var conversions []*ConversionRange
	for scanner.Scan() {
		rawLine := scanner.Text()
		// if rawLine == "" {
		// 	conversions = append(conversions, nil)
		// }
		conversions = append(conversions, parseConversionRange(rawLine))
	}

	minConvertedSeedNum := math.MaxInt
	startSeed := 0
	count := 0
	var seedRange SeedRange
	var allSeedRanges []SeedRange
	for len(strings.Trim(rawSeedLine, " ")) > 0 {
		seedRange, rawSeedLine = parseOneSeedRange(rawSeedLine)
		allSeedRanges = append(allSeedRanges, seedRange)
	}

	removeDuplicateSeeds(allSeedRanges)

	fmt.Println(allSeedRanges)

	for _, seedRange := range allSeedRanges {
		fmt.Printf("start: %v, count: %v\n", startSeed, count)
		for seedNum := seedRange.start; seedNum <= seedRange.end; seedNum++ {
			//fmt.Println(seedNum)
			convertedSeedNum := convertSeed(seedNum, conversions)
			if convertedSeedNum < minConvertedSeedNum {
				minConvertedSeedNum = convertedSeedNum
			}
		}
	}
	fmt.Println(minConvertedSeedNum)

	//fmt.Println(conversions)
}

func removeDuplicateSeeds(allSeedRanges []SeedRange) {
	for _, r1 := range allSeedRanges {
		for _, r2 := range allSeedRanges {
			if r1.start >= r2.start && r1.start <= r2.end {
				r1.start = r2.end
				fmt.Println("start brought up")
			}
			if r1.end >= r2.start && r1.end <= r2.end {
				r1.end = r2.start
				fmt.Println("end brought down")
			}
		}
	}
}

func parseOneSeedRange(s string) (SeedRange, string) {
	seedStrings := strings.SplitN(s, " ", 3)
	//fmt.Println(seedStrings[0], " - ", seedStrings[1], " - ", seedStrings[2])
	start, err := strconv.Atoi(seedStrings[0])
	if err != nil {
		log.Fatal(err)
	}
	count, err := strconv.Atoi(seedStrings[1])
	if err != nil {
		log.Fatal(err)
	}
	var remaining string
	if len(seedStrings) == 3 {
		remaining = seedStrings[2]
	}

	return SeedRange{
		start: start,
		end:   start + count - 1,
	}, remaining
}

func parseConversionRange(s string) *ConversionRange {
	vals := strings.Split(s, " ")
	var destStart int
	var sourceStart int
	var size int
	count := 0
	for _, val := range vals {
		num, err := strconv.Atoi(val)
		if err != nil {
			continue
		}
		if count == 0 {
			destStart = num
		} else if count == 1 {
			sourceStart = num
		} else if count == 2 {
			size = num
			//fmt.Printf("destStart: %v, sourceStart: %v, size: %v\n", destStart, sourceStart, size)
		}
		count++
	}
	if count != 3 {
		return nil
	}
	return &ConversionRange{
		start:  sourceStart,
		end:    sourceStart + size,
		offset: destStart - sourceStart,
	}
}

func convertSeed(n int, conversionRanges []*ConversionRange) int {
	allowConversion := true
	convertedVal := n
	for _, conversion := range conversionRanges {

		if conversion == nil {
			allowConversion = true
			continue
		}
		//fmt.Println("start: ", conversion.start, " - end: ", conversion.end, " - offset: ", conversion.offset)
		if !allowConversion {
			continue
		}
		if convertedVal >= conversion.start && convertedVal < conversion.end {
			//fmt.Print("Converting ", convertedVal, " to ")
			convertedVal += conversion.offset
			//fmt.Println(convertedVal)
			allowConversion = false
		}

	}
	return convertedVal
}

//func
