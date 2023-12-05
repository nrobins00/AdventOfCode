package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

type Data struct {
    Lines []string
}

type NumberMap struct {
    start int
    end int
    offset int
}

type InputRange struct {
    start int
    end int
}

func main() {
    buf, err := os.ReadFile("input.txt")
    if err != nil {
        log.Fatal(err)
    }
    //d := &Data{}
    //var allSeedNums [][]int

    lines := strings.Split(string(buf), "\n")
    fmt.Println(lines[0])
    seedLine := lines[0]

    seeds := strings.SplitAfter(seedLine, " ")


    var isStart bool     
    var lastSeed int
    allTimeMin := 99999999999
    //var inputRanges []inputRange
    for i, seedString := range(seeds) {
        var seedNums []int
        if i % 2 != 0 {
            isStart = true
        } else {
            isStart = false
            
        }
        seedString = strings.TrimSpace(seedString)
        seedNum, err := strconv.Atoi(seedString)
        if err != nil {
            fmt.Println(err)
            continue
        }
        if isStart {
            lastSeed = seedNum
        } else {
            //for _, numRange := range inputRanges {
            //    if numInRange(lastSeed, numRange) {
            //        newEnd := lastSeed + seedNum
            //        if newEnd > numRange.end {
            //            numRange.end = newEnd
            //        }
            //        break
            //    }

            //}
            seedNums := make([]int, seedNum)
            fmt.Println(seedNum)
            count := 0
            for i := 0; i < seedNum; i++ {
                count += lastSeed
                //seedNums = append(seedNums, lastSeed + i)
            }
            var seedAllowances []bool
            for range(seedNums) {
                seedAllowances = append(seedAllowances, true)
            }
            for lineNum, line := range lines {
                if line == "" || lineNum == 0 {
                    for index, _ := range(seedAllowances) {
                        seedAllowances[index] = true
                    }
                    continue

                }
                numMap, isValid := parseMap(line)
                if !isValid {
                    continue
                }
                for index, seedNum := range seedNums {
                    if seedNum >= numMap.start && seedNum <= numMap.end {
                        if seedAllowances[index] {
                            seedNums[index] = seedNum + numMap.offset
                            seedAllowances[index] = false
                        }
                    }
                }

            }

            min := 999999999999999
            for _, seedNum := range seedNums {
                if seedNum < min {
                    min = seedNum
                    fmt.Println("newMin: ", min)
                }
            }
            if min < allTimeMin {
                allTimeMin = min
            }
            seedNums = seedNums[:0]
        }
        fmt.Println(i)
        fmt.Println("capacity: ", cap(seedNums))
        //fmt.Println(seedNums)
    }
    fmt.Println(allTimeMin)
    //for _, seedNums := range(allSeedNums) {

    //    var seedAllowances []bool
    //    for range(seedNums) {
    //        seedAllowances = append(seedAllowances, true)
    //    }
    //    for lineNum, line := range lines {
    //        if line == "" || lineNum == 0 {
    //            for index, _ := range(seedAllowances) {
    //                seedAllowances[index] = true
    //            }
    //            continue

    //        }
    //        numMap, isValid := parseMap(line)
    //        if !isValid {
    //            continue
    //        }
    //        for index, seedNum := range seedNums {
    //            if seedNum >= numMap.start && seedNum <= numMap.end {
    //                if seedAllowances[index] {
    //                    seedNums[index] = seedNum + numMap.offset
    //                    seedAllowances[index] = false
    //                }
    //            }
    //        }

    //    }

    //    min := 999999999999999
    //    for _, seedNum := range seedNums {
    //        if seedNum < min {
    //            min = seedNum
    //        }
    //    }
    //    if min < allTimeMin {
    //        allTimeMin = min
    //    }
    //}
    //fmt.Println(allTimeMin)

    //fmt.Println("aout")
}

//func findMinimum(seedNums []int) int {
//
//        var seedAllowances []bool
//        for range(seedNums) {
//            seedAllowances = append(seedAllowances, true)
//        }
//        for lineNum, line := range lines {
//            if line == "" || lineNum == 0 {
//                for index, _ := range(seedAllowances) {
//                    seedAllowances[index] = true
//                }
//                continue
//
//            }
//            numMap, isValid := parseMap(line)
//            if !isValid {
//                continue
//            }
//            for index, seedNum := range seedNums {
//                if seedNum >= numMap.start && seedNum <= numMap.end {
//                    if seedAllowances[index] {
//                        seedNums[index] = seedNum + numMap.offset
//                        seedAllowances[index] = false
//                    }
//                }
//            }
//
//        }
//
//        min := 999999999999999
//        for _, seedNum := range seedNums {
//            if seedNum < min {
//                min = seedNum
//            }
//        }
//        if min < allTimeMin {
//            allTimeMin = min
//        }
//}

func numInRange(num int, numRange InputRange) bool {
    return num <= numRange.end && num >= numRange.start
}


func parseMap(s string) (*NumberMap, bool)  {
    numberStrings := strings.Split(s, " ")
    count := 0
    var destStart int
    var sourceStart int
    var trRange int 
    if s[0] < '0' || s[0] > '9' {
        return nil, false
    }
    for _, str := range (numberStrings) {
        str = strings.TrimSpace(str)
        num, err := strconv.Atoi(str)
        if err != nil {
            continue
        }
        if count == 0 {
            destStart = num
        }
        if count == 1 {
            sourceStart = num
        }
        if count == 2 {
            trRange = num
        }
        count = count + 1
    }
    offSet := destStart - sourceStart
    start := sourceStart
    end := start + trRange
    //fmt.Println(destStart, " ", sourceStart, " ", trRange)
    return &NumberMap{
       start: start, 
       end: end,
       offset: offSet,
    }, true

}
 

