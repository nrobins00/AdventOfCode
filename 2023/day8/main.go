package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"
)

type Node struct {
	label string
	left  string
	right string
}

func main() {
	data := parseInput("input.txt")
	instructionString := data[0]

	fmt.Println(instructionString)

	mapLines := data[1:]
	nodes := make(map[string]Node)
	for _, line := range mapLines {
		node := parseMapLine(line)
		nodes[node.label] = node
	}

	workingNodes := getStartNodes(nodes)
	fmt.Println(workingNodes)

	zIndices := make([]int, len(workingNodes))

	instructionIndex := 0
	for !allNodesFinished(workingNodes, instructionIndex/len(instructionString), zIndices) {
		//fmt.Println(instructionIndex)
		instruction := string(instructionString[instructionIndex%len(instructionString)])

		for i, node := range workingNodes {
			if instruction == "L" {
				workingNodes[i] = nodes[node.left]
			} else if instruction == "R" {
				workingNodes[i] = nodes[node.right]
			} else {
				log.Fatal("invalid instr")
			}
		}
		instructionIndex++
	}
	fmt.Println(zIndices)

	lcm := LCM(zIndices[0], zIndices[1], zIndices[2:]...)

	fmt.Println(instructionIndex)
	fmt.Println(lcm * len(instructionString))
}

func allNodesFinished(nodes []Node, instr int, zIndices []int) bool {
	for i, node := range nodes {
		if node.label[2:] == "Z" {
			if zIndices[i] == 0 {
				zIndices[i] = instr
			}
		}
	}

	isFinished := true
	for _, zIndex := range zIndices {
		if zIndex == 0 {
			isFinished = false
		}
	}
	return isFinished
}

// func advanceAllNodes(nodes []Node, direction string) {
// 	for i, node := range nodes {
// 		if direction == "L" {
// 			nodes[i] = nodes[i].left
// 		}
// 	}
// }

func getStartNodes(nodes map[string]Node) []Node {
	var startNodes []Node
	for label, node := range nodes {
		if label[2:] == "A" {
			startNodes = append(startNodes, node)
		}
	}
	return startNodes
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

func parseMapLine(s string) Node {
	equalsSplit := strings.Split(s, " = ")
	name := equalsSplit[0]
	pointersString := equalsSplit[1]
	left := pointersString[1:4]
	right := pointersString[6:9]
	return Node{
		label: name,
		left:  left,
		right: right,
	}
}

// greatest common divisor (GCD) via Euclidean algorithm
func GCD(a, b int) int {
	for b != 0 {
		t := b
		b = a % b
		a = t
	}
	return a
}

// find Least Common Multiple (LCM) via GCD
func LCM(a, b int, integers ...int) int {
	fmt.Println(a, " ", b)
	result := a * b / GCD(a, b)

	for i := 0; i < len(integers); i++ {
		result = LCM(result, integers[i])
	}

	return result
}
