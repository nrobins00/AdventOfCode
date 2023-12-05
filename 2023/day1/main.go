package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

func main() {
        sum := 0    
        f, err := os.Open("./input.txt")
        if err != nil {
            log.Fatal(err)
        }
        defer f.Close()
        
        scanner := bufio.NewScanner(f)
        var firstNum int
        var lastNum int
        var firstFound bool

        stringDigitMap := make(map[string]int)
        stringDigitMap["one"] = 1
        stringDigitMap["two"] = 2
        stringDigitMap["three"] = 3
        stringDigitMap["four"] = 4
        stringDigitMap["five"] = 5
        stringDigitMap["six"] = 6
        stringDigitMap["seven"] = 7
        stringDigitMap["eight"] = 8
        stringDigitMap["nine"] = 9

        for scanner.Scan() {
            line := scanner.Text()
            firstNum = 0
            lastNum = 0
            firstFound = false
            for i := 0; i < len(line); i++ {
                char := line[i]
                if char > 47 && char < 58 {
                    //fmt.Println(char)
                    if !firstFound {
                        firstFound = true
                        firstNum = int(char - '0')
                        lastNum = firstNum
                    } else {
                        lastNum = int(char - '0') 
                    }
                } else {
                    potentialWordDigit := []byte{char}
                    //potentialWordDigit = append(potentialWordDigit, char)
                    for j := i + 1; j <len(line); j++ {
                        nextChar := line[j]
                        potentialWordDigit = append(potentialWordDigit, nextChar)
                        //fmt.Println(string(potentialWordDigit))
                        val, prs := stringDigitMap[string(potentialWordDigit)] 
                        //fmt.Println("VAL, PRS: ", val, prs)
                        if prs {
                            if !firstFound {
                                firstFound = true
                                firstNum = val
                                lastNum = firstNum
                            } else {
                                lastNum = val
                            }
                            fmt.Println("VAl, prs", val, prs)
                            break
                        } 
                    }
                }

            }
            fmt.Printf("firstNum: %v \n", firstNum)
            fmt.Printf("secondNum: %v \n", lastNum)
            sum = sum + (firstNum) * 10 + lastNum
            fmt.Println(stringDigitMap["two"])
        }
        
        fmt.Println(sum)

        if err := scanner.Err(); err != nil {
            log.Fatal(err)
        }
}

