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

const (
	Up    int = 0
	Right     = 1
	Down      = 2
	Left      = 3
)

type Vertex struct {
	row int
	col int
}

type Approach struct {
	direction  int //direction enum
	lineLength int //num steps in this direction
}

type VertexApproach struct {
    vertex Vertex
    approach Approach
}

type Grid [][]int

func main() {
	data := parseInput("input.txt")

	fmt.Println(data)

    grid := Grid(data) 

	spt := make(map[VertexApproach]int)

	startVertex := Vertex{row: 0, col: 0}
	startApproach := Approach{}
    startVertexApproach := VertexApproach{ startVertex, startApproach}
	//endVertex := Vertex{len(data) - 1, len(data[0]) - 1}

    spt[startVertexApproach] = 0
    frontier := make(map[VertexApproach]int)

    downVertex := startVertex.stepInDirection(Down)
    downCost := grid[downVertex.row][downVertex.col]
    downVertexApproach := VertexApproach{
        vertex: downVertex,
        approach: Approach{direction: Down, lineLength: 1},
    }
    frontier[downVertexApproach] = downCost

    rightVertex := startVertex.stepInDirection(Right)
    rightCost := grid[rightVertex.row][rightVertex.col]
    rightVertexApproach := VertexApproach{
        vertex: rightVertex,
        approach: Approach{direction: Right, lineLength: 1},
    }
    frontier[rightVertexApproach] = rightCost

    var endCost int
    var foundEnd bool
    //step := 0
    for !foundEnd  {
        //fmt.Println(frontier)
        nextTarget := pickClosestFromFrontier(frontier)
        //fmt.Println(nextTarget)
        cost := frontier[nextTarget]
        delete(frontier, nextTarget)
        //fmt.Println(nextTarget, " ", cost)
        spt[nextTarget] = cost
        frontier = grid.addAdjacentNodesToFrontier(spt, frontier, nextTarget, cost)
        foundEnd, endCost = grid.doesSptContainEnd(spt)
        //fmt.Println(frontier)
        //fmt.Println("------------------")
        //step++
    }

    //fmt.Println(spt)

    fmt.Println(endCost)



	//for _, prs := spt[endVertex]; !prs; _, prs = spt[endVertex] {
    //    
	//}

	//fmt.Println(cost)
}

//func findShortestVertexApproach(frontier map[VertexApproach]int, g Grid)  {}

func (g Grid) doesSptContainEnd(spt map[VertexApproach]int) (bool, int) {
    //possibilities for end
    endVertex := Vertex{
        row: len(g)-1,
        col: len(g[0])-1,
    }
    endVa := VertexApproach {
        vertex: endVertex,
        approach: Approach{},
    }
    for dir:=0; dir<4; dir++ {
        endVa.approach.direction = dir
        for len:=4; len<11; len++ {
            endVa.approach.lineLength = len
            cost, prs := spt[endVa]
            if prs {
                return true, cost
            }
        }
    }
    return false, 0
}

func (g Grid) addAdjacentNodesToFrontier(spt map[VertexApproach]int, frontier map[VertexApproach]int, va VertexApproach, currentCost int) map[VertexApproach]int {
    direction := va.approach.direction
    //fmt.Println(direction)
    //fmt.Println(va)
    if va.approach.lineLength < 10 {
        nextStart := va.vertex.stepInDirection(direction)
        if g.isVertexValid(nextStart) {
            nextCost := currentCost + g[nextStart.row][nextStart.col]
            newApproach := va.approach
            newApproach.lineLength++
            straightVa := VertexApproach {
                vertex: nextStart,
                approach: newApproach,
            }
            _, prs := spt[straightVa]
            if !prs {
                existingCost, prs := frontier[straightVa]
                if prs {
                    frontier[straightVa] = min(nextCost, existingCost)
                } else {
                    frontier[straightVa] = nextCost
                }
            }
            //fmt.Println("straight: ", straightVa, " ,cost: ", nextCost) 
        }
    }
    if va.approach.lineLength >= 4 {
        leftDirection := mod(direction - 1, 4)
        leftStart := va.vertex.stepInDirection(leftDirection)
        if g.isVertexValid(leftStart) {
            nextCost := currentCost + g[leftStart.row][leftStart.col]
            newApproach := Approach{leftDirection, 1}
            newVa := VertexApproach {
                vertex: leftStart,
                approach: newApproach,
            }
            _, prs := spt[newVa]
            if !prs {
                existingCost, prs := frontier[newVa]
                if prs {
                    frontier[newVa] = min(nextCost, existingCost)
                } else {
                    frontier[newVa] = nextCost
                }
            }
            //fmt.Println("left: ", newVa, " ,cost: ", nextCost) 
        }
    

        rightDirection := mod(direction + 1, 4)
        rightStart := va.vertex.stepInDirection(rightDirection)
        if g.isVertexValid(rightStart) {
            nextCost := currentCost + g[rightStart.row][rightStart.col]
            //fmt.Println(currentCost, " + ", g[rightStart.row][rightStart.col])
            newApproach := Approach{rightDirection, 1}
            newVa := VertexApproach {
                vertex: rightStart,
                approach: newApproach,
            }
            _, prs := spt[newVa]
            if !prs {
                existingCost, prs := frontier[newVa]
                if prs {
                    frontier[newVa] = min(nextCost, existingCost)
                } else {
                    frontier[newVa] = nextCost
                }
            }
            //fmt.Println("right: ", newVa, " ,cost: ", nextCost)
        }
    }
    //fmt.Println("-------------------------------")
    return frontier
}

func (g Grid) isEndVertex(v Vertex) bool {
    return v.row == len(g) - 1 && v.col == len(g[0]) - 1
}

func  pickClosestFromFrontier(frontier map[VertexApproach]int) VertexApproach {
    smallestCost := math.MaxInt
    var smallestVa VertexApproach
    for va, cost := range frontier {
       if cost < smallestCost {
            smallestCost = cost
            smallestVa = va
        }
    }
    return smallestVa
}

func (g Grid) findShortestCostOuter(start Vertex, end Vertex) int {
    emptyMemo := make(map[string]int)
    downStart := start.stepInDirection(Down)
    nextCost := g[downStart.row][downStart.col]
    downCost := g.findShortestCost(downStart, end, Down, 1, nextCost, emptyMemo)

    rightStart := start.stepInDirection(Right)
    nextCost = g[rightStart.row][rightStart.col]
    rightCost := g.findShortestCost(rightStart, end, Right, 1, nextCost, emptyMemo)

    return min(downCost, rightCost)
}


func (g Grid) findShortestCost(start Vertex, end Vertex, direction int, lineLen int, currentCost int, memo map[string]int) int {
    uniqStr := strconv.Itoa(start.row) + ":" + strconv.Itoa(start.col) + strconv.Itoa(direction) + strconv.Itoa(lineLen) + strconv.Itoa(currentCost)
    cost, prs := memo[uniqStr]
    if prs {
        return cost
    }
    if start.col == end.col && start.row + 1 == end.row {
        return currentCost + g[end.row][end.col]
    }
    if start.row == end.row && start.col + 1 == end.col {
        return currentCost + g[end.row][end.col]
    }
    var straightCost int
    var leftCost int
    var rightCost int
    if lineLen < 3 {
        nextStart := start.stepInDirection(direction)
        if g.isVertexValid(nextStart) {
            nextCost := currentCost + g[nextStart.row][nextStart.col]
            straightCost = g.findShortestCost(nextStart, end, direction, lineLen + 1, nextCost, memo)
        }
    }
    leftDirection := mod(direction - 1, 4)
    leftStart := start.stepInDirection(leftDirection)
    if g.isVertexValid(leftStart) {
        nextCost := currentCost + g[leftStart.row][leftStart.col]
        leftCost = g.findShortestCost(leftStart, end, leftDirection, 0, nextCost, memo)
    }

    rightDirection := mod(direction + 1, 4)
    rightStart := start.stepInDirection(rightDirection)
    if g.isVertexValid(rightStart) {
        nextCost := currentCost + g[rightStart.row][rightStart.col]
        rightCost = g.findShortestCost(rightStart, end, rightDirection, 0, nextCost, memo)
    }
    if straightCost == 0 { straightCost = math.MaxInt }
    if leftCost == 0 { leftCost = math.MaxInt }
    if rightCost == 0 { rightCost = math.MaxInt }
    
    totalMin := min(straightCost, leftCost, rightCost)
    memo[uniqStr] = totalMin
    return totalMin
}

func (g Grid) isVertexValid(v Vertex) bool {
    return !(v.row < 0 || v.row >= len(g) || v.col < 0 || v.col >= len(g[0]))
}

func (v Vertex) stepInDirection(direction int) Vertex {
    newVert := v
    if direction == Down {
        newVert.row++
    } else if direction == Up {
        newVert.row--
    } else if direction == Left {
        newVert.col--
    } else if direction == Right {
        newVert.col++
    }
    return newVert
}

func parseInput(fileName string) [][]int {
	buf, err := os.Open(fileName)
	if err != nil {
		log.Fatal(err)
	}
	scanner := bufio.NewScanner(buf)

	var ret [][]int
	for scanner.Scan() {
		line := scanner.Text()
		if line != "" {
			lineSlice := strings.Split(line, "")
			var lineIntSlice []int
			for _, str := range lineSlice {
				num, err := strconv.Atoi(str)
				if err != nil {
					log.Fatal(err)
				}
				lineIntSlice = append(lineIntSlice, num)
			}
			ret = append(ret, lineIntSlice)
		}
	}
	return ret
}

func mod(a, b int) int {
    m := a % b
    if a < 0 && b < 0 {
        m -= b
    }
    if a <  0 && b > 0 {
        m += b
    }
    return m
}

