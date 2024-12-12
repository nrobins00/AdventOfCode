#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

struct Stone {
	uint64_t no;
	uint64_t quant;
};

struct Stones {
	struct Stone *arr;
	uint64_t capacity;
	//uint64_t numStones;
	uint32_t distinctCount;
};

void expandStones(struct Stones *stones) {
	int oldSize = stones->capacity;
	int newSize = oldSize * 2;
	//printf("expanding stones to size %d\n", newSize);
	struct Stone *newArr = calloc(newSize, sizeof(struct Stone));
	if (newArr == NULL) {
		printf("couldn't expand to size %d\n", newSize);
	}
	struct Stone *oldArr = stones->arr;
	memcpy(newArr, oldArr, oldSize * sizeof(struct Stone));
	free(oldArr);
	stones->arr = newArr;
	stones->capacity = newSize;
	//printf("done expanding stones\n");
	//printf("capacity = %lu\n", stones->capacity);
}

int countDigits(uint64_t currStone) {
	int numDigits = 1;
	while (currStone >= 10) {
		numDigits++;
		currStone /= 10;
	}

	return numDigits;
}

void splitStone(uint64_t currStone, uint64_t *leftStone, uint64_t *rightStone) {
	int numDigits = 0;
	int digitsInStone = countDigits(currStone);
	while (numDigits < digitsInStone / 2) {
		*rightStone += (currStone % 10) * powl(10, numDigits);
		currStone /= 10;
		numDigits++;
	}

	*leftStone = currStone;
}

int findIndexOfStone(struct Stones *stones, uint64_t stoneNumber) {
	for (int i=0; i<stones->distinctCount; i++) {
		if (stones->arr[i].no == stoneNumber) {
			return i;
		}
	}
	return -1;
}

int addStone(struct Stones *stones, uint64_t stoneNumber) {
	if (stones->distinctCount >= stones->capacity) {
		expandStones(stones);
	}

	struct Stone newStone;
	newStone.no = stoneNumber;
	newStone.quant = 0;
	stones->arr[stones->distinctCount++] = newStone;
	return stones->distinctCount-1;
}

void incrementStones(struct Stones *stones, uint64_t stoneNumber, uint64_t numToAdd) {
	//printf("incrementStones: stoneNumber=%lu, numToAdd=%lu\n", stoneNumber, numToAdd);
	int index = findIndexOfStone(stones, stoneNumber);

	if (index == -1) {
		index = addStone(stones, stoneNumber);
	}

	stones->arr[index].quant += numToAdd;
}

void blink(struct Stones *stones) {
	uint64_t i=0;
	struct Stones newStones;
	newStones.capacity = stones->capacity;
	newStones.arr = calloc(newStones.capacity, sizeof(struct Stone));
	newStones.distinctCount = 0;
	while (i < stones->distinctCount) { 
		struct Stone currStone = stones->arr[i];

		//printf("currStone: %lu; numStones: %d\n", currStone, numStones);
		if (currStone.no == 0) {
			incrementStones(&newStones, 1, currStone.quant);
		} else if (countDigits(currStone.no) % 2 == 0) {
			uint64_t leftStone = 0;
			uint64_t rightStone = 0;
			splitStone(currStone.no, &leftStone, &rightStone);
			incrementStones(&newStones, leftStone, currStone.quant);
			incrementStones(&newStones, rightStone, currStone.quant);
		} else {
			uint64_t toInsert = currStone.no*2024;
			incrementStones(&newStones, toInsert, currStone.quant);
		}
		++i;
	}
	//printf("numStones: %lu\n", newStones.numStones);
	free(stones->arr);
	*stones = newStones;
}

int main(int argc, char **argv) {
	FILE *file = fopen(argv[1], "r");
	int numBlinks = atoi(argv[2]);

	struct Stones stones;
	stones.capacity = 10;
	stones.distinctCount=0;
	stones.arr = calloc(stones.capacity, sizeof(struct Stone));
	char currChar;
	char curString[10];
	int strIdx;
	int currNum;
	while ((currChar = fgetc(file)) != EOF) {
		switch (currChar) {
		case '\n':
			curString[strIdx] = '\0';
			currNum = strtoull(curString, NULL, 10);
			incrementStones(&stones, currNum, 1);
			break;
		case ' ':
			curString[strIdx] = '\0';
			strIdx = 0;
			currNum = strtoull(curString, NULL, 10);
			incrementStones(&stones, currNum, 1);
			break;
		default:
			curString[strIdx++] = currChar;
		}
	}
	fclose(file);
	//for (uint64_t i=0; i<stones.capacity; i++) {
	//	if (stones.arr[i] > 0) {
	//		printf("%d %d stones\n", stones.arr[i], i);
	//	}
	//}

	for (int i=0; i<numBlinks; i++) {
		blink(&stones);
		//printf("blink %d: numStones: %d\n", i+1, stones.numStones);
	}
	
	//printf("%lu\n", stones.capacity);
	//for (int i=0; i<stones.capacity; i++) {
	//	if (stones.arr[i] > 0) {
	//		printf("%d %d stones\n", stones.arr[i], i);
	//	}
	//}
	//printf("num stones: %lu\n", stones.numStones);
	uint64_t total = 0;
	for (int i=0; i<stones.distinctCount; i++) {
		total += stones.arr[i].quant;
	}
	printf("total stones: %lu\n", total);


	free(stones.arr);
}
