package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"
    "strconv"
)

func main() {
    f, err := os.Open("./input.txt")    
    if err != nil {
        log.Fatal(err)
    }
    defer f.Close()

    scanner := bufio.NewScanner(f)

    maxes := map[string]int{
        "red": 12,
        "green": 13,
        "blue": 14,
    }

    sum := 0
    powerSum := 0
    for scanner.Scan() {
        line := scanner.Text()
        if len(line) < 1 {
            continue
        }
        gameNum := 0
        gameIsGood := true
        pieces := strings.Split(line, " ")
        currentMaxes := map[string]int{
            "red": 0,
            "green": 0,
            "blue": 0,
        }
        for index, val := range pieces {
            if colonIndex := strings.Index(val, ":"); colonIndex > 0 {
                gameNum, err = strconv.Atoi(val[:colonIndex]) 
                if err != nil {
                    log.Fatal(err)
                }
            
            } else {
                count, err := strconv.Atoi(val)
                if err != nil {
                    continue
                }

                color := strings.Trim(pieces[index+1], ";,")
                if count > maxes[color] {
                    fmt.Println(gameNum, " ", count, " ", color)
                    gameIsGood = false
                }
                if count > currentMaxes[color] {
                    currentMaxes[color] = count
                }
            }
        }
        
        if gameIsGood {
            fmt.Println(gameNum)
            sum = sum + gameNum
        }
        power := 1
        for _, val := range currentMaxes {
           power = power * val 
        }
        powerSum = powerSum + power
    }
    fmt.Println("Part one ", sum)
    fmt.Println("Part two ", powerSum)

}
