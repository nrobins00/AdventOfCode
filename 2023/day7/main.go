package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

type Hand struct {
	hand    string
	handVal int
	bid     int
}

func main() {
	data := parseInput("input.txt")

	var rankings []Hand

	for _, rawLine := range data {
		rawSlice := strings.Split(rawLine, " ")

		hand := rawSlice[0]
		bid, err := strconv.Atoi(rawSlice[1])

		if err != nil {
			log.Fatal(err)
		}

		newHand := Hand{
			hand:    hand,
			bid:     bid,
			handVal: getValueForHand(hand),
		}

		placeToPutNewHand := -1
		for key, val := range rankings {
			if newHand.handVal > val.handVal {
				placeToPutNewHand = key
				rankings = placeNewHand(rankings, placeToPutNewHand, newHand)
				break
			}
			if newHand.handVal == val.handVal {
				//newHandIsGreater := false
				//oldHandIsGreater := false
				existingHand := val.hand
				for i, newHandCharVal := range newHand.hand {
					compare := findGreaterCard(newHandCharVal, rune(existingHand[i]))
					if compare == 1 {
						placeToPutNewHand = key
						rankings = placeNewHand(rankings, placeToPutNewHand, newHand)
						break
					} else if compare == -1 {
						break
					}
				}
			}
			if placeToPutNewHand > -1 {
				break
			}
		}
		if placeToPutNewHand == -1 {
			rankings = placeNewHand(rankings, placeToPutNewHand, newHand)
		}
	}

	winnings := 0
	multiplier := len(rankings)
	for _, hand := range rankings {
		winnings += hand.bid * multiplier
		multiplier--
	}
	fmt.Println(winnings)
	//get hand
	// get bid

}

func placeNewHand(rankings []Hand, place int, newHand Hand) []Hand {
	if place == -1 {
		rankings = append(rankings, newHand)
	} else {
		var handsAfter []Hand
		handsAfter = append(handsAfter, rankings[place:]...)
		rankings = append(rankings[0:place], newHand)
		rankings = append(rankings, handsAfter...)
	}
	return rankings
}

func findGreaterCard(a rune, b rune) int {
	cardRanks := "J23456789TQKA"
	aIndex := strings.Index(cardRanks, string(a))
	bIndex := strings.Index(cardRanks, string(b))
	if aIndex > bIndex {

		return 1
	} else if bIndex > aIndex {
		return -1
	} else {
		return 0
	}
}

func getValueForHand(hand string) int {
	if isFiveOfAKind(hand) {
		return 6
	}
	if isFourOfAKind(hand) {
		return 5
	}
	if isFullHouse(hand) {
		return 4
	}
	if isThreeOfAKind(hand) {
		return 3
	}
	if isTwoPair(hand) {
		return 2
	}
	if isOnePair(hand) {
		return 1
	}
	return 0
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

func isFiveOfAKind(hand string) bool {

	jokerCount := strings.Count(hand, "J")
	hand = strings.ReplaceAll(hand, "J", "")
	if jokerCount == 5 {
		return true
	}
	count := strings.Count(hand, string(hand[0])) + jokerCount
	return count == 5
}

func isFourOfAKind(hand string) bool {
	isValid := false
	jokerCount := strings.Count(hand, "J")
	hand = strings.ReplaceAll(hand, "J", "")
	for _, val := range hand {
		count := strings.Count(hand, string(val)) + jokerCount
		if count >= 4 {
			isValid = true
			break
		}
	}
	return isValid
}

func isThreeOfAKind(hand string) bool {
	isValid := false
	jokerCount := strings.Count(hand, "J")
	hand = strings.ReplaceAll(hand, "J", "")
	for _, val := range hand {
		count := strings.Count(hand, string(val)) + jokerCount
		if count >= 3 {
			isValid = true
			break
		}
	}
	return isValid
}

func isFullHouse(hand string) bool {
	var pairValue string
	var threeValue string

	jokerCount := strings.Count(hand, "J")
	hand = strings.ReplaceAll(hand, "J", "")

	if jokerCount >= 3 {
		return true
	}
	if jokerCount == 2 {
		return isOnePair(hand)
	}
	if jokerCount == 1 {
		return isTwoPair(hand)
	}

	for _, val := range hand {
		count := strings.Count(hand, string(val))
		if count == 3 {
			threeValue = string(val)
		} else if count == 2 && string(val) != threeValue {
			pairValue = string(val)
		}
	}

	return pairValue != "" && threeValue != ""
}

func isTwoPair(hand string) bool {
	var pair1Val string
	var pair2Val string

	jokerCount := strings.Count(hand, "J")
	hand = strings.ReplaceAll(hand, "J", "")

	if jokerCount > 1 {
		return true
	}

	if jokerCount == 1 {
		return isOnePair(hand)
	}

	for _, val := range hand {
		count := strings.Count(hand, string(val))
		if count == 2 {
			if pair1Val == "" {
				pair1Val = string(val)
			} else if pair2Val == "" && string(val) != pair1Val {
				pair2Val = string(val)
			}
		}
	}
	return pair1Val != "" && pair2Val != ""
}

func isOnePair(hand string) bool {
	jokerCount := strings.Count(hand, "J")
	hand = strings.ReplaceAll(hand, "J", "")
	if jokerCount > 0 {
		return true
	}

	for _, val := range hand {
		count := strings.Count(hand, string(val))
		if count == 2 {
			return true
		}
	}
	return false
}
