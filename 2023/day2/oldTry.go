package main
//
//import (
//	"bufio"
//	"fmt"
//	"log"
//	"os"
//	"strings"
//    "strconv"
//)
//
//func oldMain() {
//    f, err := os.Open("./input.txt")    
//    if err != nil {
//        log.Fatal(err)
//    }
//    defer f.Close()
//
//    scanner := bufio.NewScanner(f)
//
//    maxes := map[string]int{
//        "red": 12,
//        "green": 13,
//        "blue": 14,
//    }
//
//    //sum := 0
//    for scanner.Scan() {
//        line := scanner.Text()
//        if len(line) < 1 {
//            continue
//        }
//        gameNumAndDetailsSlice := strings.Split(line, ":")
//        gameAndNumString := gameNumAndDetailsSlice[0]
//        gameNumber, err := strconv.Atoi(gameAndNumString[5:])
//        if err != nil {
//            log.Fatal(err)
//        }
//
//        detailsString := gameNumAndDetailsSlice[1]
//        handsSlice := strings.Split(detailsString, ";")
//        gameIsGood := true
//        for i, val := range handsSlice {
//            colorsSlice := strings.Split(val, ",")
//            for _, colorAndVal := range colorsSlice {
//                 
//        }
//        if !gameIsGood {
//            break
//        }
//        
//        fmt.Println(gameNumber)
//    }
//
//}
