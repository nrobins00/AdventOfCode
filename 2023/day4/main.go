package main

import (
	"bufio"
	"log"
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

    for scanner.Scan() {
        rawLine := scanner.Text()
        winningNumbers := make([]string, 0)
        myNumbers := make([]string, 0)



    }

}

