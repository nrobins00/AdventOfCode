#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Stones {
	uint64_t *arr;
	int capacity;
	int numStones;
};

void expandStones(struct Stones *stones) {
	int oldSize = stones->capacity;
	int newSize = oldSize*2;
	uint64_t *newArr = malloc(newSize*sizeof(uint64_t));
	uint64_t *oldArr = stones->arr;
	memcpy(newArr, oldArr, oldSize * sizeof(uint64_t));
	free(oldArr);
	stones->arr = newArr;
	stones->capacity = newSize;
}

void insertAt(struct Stones *stones, uint64_t toInsert, int at) {
	if (stones->numStones+1 > stones->capacity) {
		expandStones(stones);
	}
	stones->numStones++;
	//printf("inserting %lu at %d\n", toInsert, at);

	for (int i=at; i<stones->numStones; i++) {
		uint64_t temp = stones->arr[i];
		stones->arr[i] = toInsert;
		toInsert = temp;
	}
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

void blink(struct Stones *stones) {
	int i=0;
	while (i < stones->numStones) { 
		uint64_t currStone = stones->arr[i];
		if (currStone == 0) {
			stones->arr[i] = 1;
		} else if (countDigits(currStone) % 2 == 0) {
			uint64_t leftStone = 0;
			uint64_t rightStone = 0;
			splitStone(currStone, &leftStone, &rightStone);
			stones->arr[i] = leftStone;
			++i;
			insertAt(stones, rightStone, i);
		} else {
			stones->arr[i] *= 2024;
		}
		++i;
	}
}

int main(int argc, char **argv) {
	FILE *file = fopen(argv[1], "r");

	struct Stones stones;
	stones.capacity = 10;
	stones.numStones=0;
	stones.arr = malloc(stones.capacity * sizeof(uint64_t));
	char currChar;
	char curString[10];
	int strIdx;
	int currNum;
	while ((currChar = fgetc(file)) != EOF) {
		switch (currChar) {
		case '\n':
			curString[strIdx] = '\0';
			currNum = strtoull(curString, NULL, 10);
			stones.arr[stones.numStones++] = currNum;
			break;
		case ' ':
			curString[strIdx] = '\0';
			currNum = strtoull(curString, NULL, 10);
			stones.arr[stones.numStones++] = currNum;
			strIdx = 0;
			break;
		default:
			curString[strIdx++] = currChar;
		}
	}
	fclose(file);

	for (int i=0; i< 50; i++) {
		blink(&stones);
		printf("blink %d: numStones: %d\n", i+1, stones.numStones);
	}


	free(stones.arr);



}
