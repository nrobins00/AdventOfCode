package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"os"
)

type Point struct {
    row int
    col int
    symbol string
}

type Number struct {
    row int
    start int
    end int
    val int
    counted bool
}

func main() {
    file, err := os.Open("./input.txt")
    if err != nil {
        log.Fatal(err)
    }
    br := bufio.NewReader(file)

    var symbols []Point
    var numbers []Number
    rowNumMap := make(map[int][]Number)
    currentRow := 0
    currentCol := 0
    var currentNumDigits []byte
    var currentNum *Number
    for {
        b, err := br.ReadByte()
        if err != nil {
            rowNumMap[currentRow] = numbers
            break //EoF
        }
        if b == 10 {
            //new line
            if currentNumDigits != nil {
                numVal := byteSliceToInt(currentNumDigits)
                currentNum.val = numVal
                numbers = append(numbers, *currentNum)
                currentNumDigits = nil
                currentNum = nil
            }
            rowNumMap[currentRow] = numbers
            numbers = nil 
            currentRow = currentRow + 1
            currentCol = 0
            continue
        }
        if isSymbol(b) {
            symbolPoint := Point {
                row: currentRow,
                col: currentCol,
                symbol: string(b),
            }
            symbols = append(symbols, symbolPoint)
            if currentNumDigits != nil {
                numVal := byteSliceToInt(currentNumDigits)
                currentNum.val = numVal
                numbers = append(numbers, *currentNum)
                currentNumDigits = nil
                currentNum = nil
            }
        } else if isNum(b) {
            if currentNumDigits == nil {
                currentNumDigits = make([]byte, 0)
                currentNum = new(Number) 
                currentNum.row = currentRow
                currentNum.start = currentCol
            }
            currentNum.end = currentCol
            currentNumDigits = append(currentNumDigits, b)
        } else {
            //this is a '.'
            if currentNumDigits != nil {
                numVal := byteSliceToInt(currentNumDigits)
                currentNum.val = numVal
                numbers = append(numbers, *currentNum)
                currentNumDigits = nil
                currentNum = nil
            }
        }

        currentCol = currentCol + 1
        
    }

    sum := 0
    for key, slice := range(rowNumMap) {
        fmt.Printf("row %v: ", key)
        for _, num := range(slice) {
           fmt.Print(num.val, " ")
        }
        fmt.Println()
    }

    for _, sym := range(symbols) {
        if sym.symbol != "*" {
            if err != nil {
                log.Fatal(err)
            }
            continue
        }
        adjacentNumberCount := 0
        gearRatio := 1
        possibleNums := make([]Number, 0)
        //if sym.row > 0 {
            possibleNums = append(possibleNums, rowNumMap[sym.row - 1]...)
        //}
        possibleNums = append(possibleNums, rowNumMap[sym.row]...)
        //if sym.row < len(rowNumMap) - 1 {
            possibleNums = append(possibleNums, rowNumMap[sym.row + 1]...)
        //}

        for _, num := range(possibleNums) {
            if isPointAdjacentToNum(sym, num) {
                fmt.Printf("sym: %v, numVal: %v", sym.symbol, num.val)
                fmt.Println()
                gearRatio = gearRatio * num.val
                adjacentNumberCount = adjacentNumberCount + 1
                if adjacentNumberCount == 2 {
                    //fmt.Println(num.val)
                    sum = sum + gearRatio
                    break
                }
            }
        }
    }
    fmt.Println("gear ratio: ", sum)

    //sum = 0
    //for _, num := range(numbers) {
    //    numIsValid := false
    //    for _, point := range(symbols) {
    //        if isPointAdjacentToNum(point, num) {
    //           numIsValid = true
    //           break
    //        }
    //    }
    //    if numIsValid {
    //        sum = sum + num.val
    //    }
    //}
}

func sumAdjacentNumbers(point Point, nums []Number) int {
    sum := 0
    for i, num := range(nums) {
        if num.counted {
            continue
        }
        rowDiff := point.row - num.row
        if rowDiff > 1 || rowDiff < -1 {
            continue
        }
        if point.row == num.row {
            if (point.col == num.start - 1) || (point.col == num.end + 1) {
                nums[i].counted = true
                sum = sum + num.val
            }
        } else {
            //within one row
            if point.col >= num.start - 1 && point.col <= num.end + 1 {
                nums[i].counted = true
               sum = sum + num.val 
            }
        }
    }
    return sum 
}

func isPointAdjacentToNum(point Point, num Number) bool {
    rowDiff := point.row - num.row
    if rowDiff > 1 || rowDiff < -1 {
        return false 
    }
    if point.row == num.row {
        if point.col == num.start - 1 || point.col == num.end + 1 {
            return true
        }
        return false
    }
    if point.col >= num.start - 1 && point.col <= num.end + 1 {
        return true
    }
    return false
}



func isNum(char byte) bool {
    return char > 47 && char < 58
}

func isSymbol(char byte) bool {
    isPeriod := (char == 46)
    return !isPeriod && !isNum(char)
}

func byteSliceToInt(bytes []byte) int {
    val := 0
    
    power := 0
    for i := len(bytes) - 1; i >= 0; i-- {
        digit := bytes[i] - '0'
        val = val + int(digit) * int(math.Pow10(power))
        power = power + 1
    }   
    return val
}

