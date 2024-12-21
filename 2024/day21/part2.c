#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

char numPad[4][3] = { 
			{ '7', '8', '9' },
			{ '4', '5', '6' },
			{ '1', '2', '3' },
			{ ' ', '0', 'A' }
		    };

char dirPad[2][3] = { 
			{ ' ', '^', 'A' },
			{ '<', 'v', '>' },
		    };

struct String {
	char *arr;
	uint64_t size, cap;
};

int charToNum(char c) {
	switch (c) {
	case '^':
		return 0;
		break;
	case '>':
		return 1;
		break;
	case 'v':
		return 2;
		break;
	case '<':
		return 3;
		break;
	case 'A':
		return 4;
		break;
	}
	return -1;
}

uint64_t checkMemo(uint64_t *memo, char c, int layers) {
	int index = charToNum(c);
	index = index*26 + layers;
	return memo[index];
}

void setMemo(uint64_t *memo, char c, int layers, uint64_t val) {
	int index = charToNum(c);
	index = index*26 + layers;
	memo[index] = val;
}

void printBackOneLevel(char *input) {
	int curRow = 0;
	int curCol = 2;
	for (int i=0; i<strlen(input); i++) {
		switch (input[i]) {
		case 'A':
			//printf("%c", dirPad[curRow][curCol]);
			break;
		case '^':
			curRow--;
			break;
		case '>':
			curCol++;
			break;
		case 'v':
			curRow++;
			break;
		case '<':
			curCol--;
			break;
		}
	}
	printf("\n");
}

struct String initString() {
	struct String str;
	str.size = 0;
	str.cap = 10;
	str.arr = (char *)malloc(str.cap * sizeof(char));
	return str;
}

void addToStr(struct String *str, char c) {
	if (str->size == str->cap) {
		str->cap *= 2;
		str->arr = reallocarray(str->arr, str->cap, sizeof(char));
	}

	str->arr[str->size++] = c;
}

void addStrToStr(struct String *dest, struct String src) {
	for (uint64_t i=0; i<src.size; i++) {
		addToStr(dest, src.arr[i]);
	}
}

void findNumPadPos(char c, int *row, int *col) {
	int curRow, curCol;
	for (curRow = 0; curRow < 4; curRow++) {
		for (curCol = 0; curCol < 3; curCol++) {
			if (numPad[curRow][curCol] == c) {
				*row = curRow;
				*col = curCol;
				return;
			}
		}
	}
}

void findDirPadPos(char c, int *row, int *col) {
	int curRow, curCol;
	for (curRow = 0; curRow < 2; curRow++) {
		for (curCol = 0; curCol < 3; curCol++) {
			if (dirPad[curRow][curCol] == c) {
				*row = curRow;
				*col = curCol;
				return;
			}
		}
	}
}

struct String numPadMove(int startRow, int startCol, int endRow, int endCol) {
	//printf("moving (%d, %d) to (%d, %d)\n", startRow, startCol, endRow, endCol);
	struct String moves = initString();
	int startStartRow = startRow;
	int moveDownFirst = startCol > 0 || endRow < 3;

	if (startStartRow < 3 || endCol > 0) {
		//move left
		while (startCol > endCol) {
			addToStr(&moves, '<');
			startCol--;
		}
	}
	if (moveDownFirst) {
		while (endRow > startRow) {
			addToStr(&moves, 'v');
			startRow++;
		}
	}
	//move up
	while (startRow > endRow) {
		addToStr(&moves, '^');
		startRow--;
	}

	//move right
	while (endCol > startCol) {
		addToStr(&moves, '>');
		startCol++;
	}

	while (startCol > endCol) {
		addToStr(&moves, '<');
		startCol--;
	}

	
	// move down
	while (endRow > startRow) {
		addToStr(&moves, 'v');
		startRow++;
	}

	return moves;
}

struct String dirPadMove(int startRow, int startCol, int endRow, int endCol) {
	struct String moves = initString();

	int moveLeftFirst = startRow > 0 || (endCol >= 1);
	int moveUpFirst = startCol > 0 || endRow == 1;

	if (moveLeftFirst) {
		while (startCol > endCol) {
			addToStr(&moves, '<');
			startCol--;
		}
	}

	if (moveUpFirst) {
		while (startRow > endRow) {
			addToStr(&moves, '^');
			startRow--;
		}
	}
	// move down
	while (endRow > startRow) {
		addToStr(&moves, 'v');
		startRow++;
	}

	//move right
	while (endCol > startCol) {
		addToStr(&moves, '>');
		startCol++;
	}

	//move left
	while (startCol > endCol) {
		addToStr(&moves, '<');
		startCol--;
	}
	
	//move up
	while (startRow > endRow) {
		addToStr(&moves, '^');
		startRow--;
	}
	return moves;
}

struct String findNumPadInput(char *goal) {
	int curRow = 3;
	int curCol = 2;
	struct String allMoves = initString();
	for (int i=0; i<strlen(goal); i++) {
		int nextRow;
		int nextCol;
		findNumPadPos(goal[i], &nextRow, &nextCol);
		struct String moves = numPadMove(curRow, curCol, nextRow, nextCol);
		addToStr(&moves, 'A');
		addStrToStr(&allMoves, moves);
		free(moves.arr);
		curRow = nextRow;
		curCol = nextCol;
	}
	addToStr(&allMoves, '\0');
	//printf("%s\n", allMoves.arr);
	return allMoves;
}

struct String findDirPadInput(char *goal, char startC) {
	int curRow;
	int curCol;
	findDirPadPos(startC, &curRow, &curCol);

	struct String allMoves = initString();
	for (int i=0; i<strlen(goal); i++) {
		int nextRow;
		int nextCol;
		findDirPadPos(goal[i], &nextRow, &nextCol);
		struct String moves = dirPadMove(curRow, curCol, nextRow, nextCol);
		addToStr(&moves, 'A');
		addStrToStr(&allMoves, moves);
		free(moves.arr);
		curRow = nextRow;
		curCol = nextCol;
	}
	addToStr(&allMoves, '\0');
	//printf("%s\n", allMoves.arr);
	return allMoves;
}

uint64_t countInputsNeeded(char c, char *curPos, int numLayers, uint64_t *memo) {
	if (curPos[numLayers] == 'A') {
		//printf("using cached value!\n");
		uint64_t memVal = checkMemo(memo, c, numLayers);
		if (memVal > 0) {
			curPos[numLayers] = c;
			return memVal;
		}
	}
	//printf("countInputsNeeded: to %c, from %c, %d\n", c, curPos[numLayers], numLayers);
	if (numLayers == 0) {
		if (curPos[numLayers] == 'A') {
			setMemo(memo, c, numLayers, 1);
		}
		return 1;
	}
	//uint64_t memoVal = checkMemo(memo, c, numLayers);
	//if (memoVal > 0) {
	//	curPos[numLayers] = c;
	//	return memoVal;
	//}

	char strToFind[2] = { c, '\0' };

	struct String nextLevel = findDirPadInput(strToFind, curPos[numLayers]);
	//printf("now checking %s at %d layers\n", nextLevel.arr, numLayers-1);
	uint64_t total = 0;
	for (uint64_t i=0; i<nextLevel.size-1; i++) {
		total += countInputsNeeded(nextLevel.arr[i], curPos, numLayers-1, memo);
		curPos[numLayers-1] = nextLevel.arr[i];
	}
	free(nextLevel.arr);
	if (curPos[numLayers] == 'A') {
		setMemo(memo, c, numLayers, total);
	}
	return total;
}


int main(int argc, char **argv) {
	FILE *file = fopen(argv[1], "r");
	//char *str = "<A>Av<<AA>^AA>AvAA^A<vAAA>^A";
	//printBackOneLevel(str);
	//fclose(file);
	//exit(0);

	char codes[5][5];
	char currChar;
	int currCode = 0;
	int currCodeIdx = 0;
	while ((currChar = fgetc(file)) != EOF) {
		if (currChar == '\n') {
			codes[currCode][currCodeIdx] = '\0';
			currCode++;
			currCodeIdx = 0;
		} else {
			codes[currCode][currCodeIdx++] = currChar;
		}
	}
	fclose(file);

	uint64_t memo[700]; // really only needs to be 26*4=104
	for (int i=0; i<700; i++) {
		memo[i]=0;
	}
	//memo[0] = 1;
	//memo[1*26] = 1;
	//memo[2*26] = 1;
	//memo[3*26] = 1;
	//memo[4*26] = 1;

	char curPos[26];
	for (int i=0; i<26; i++) {
		curPos[i] = 'A';
	}

	int numRobotsWithDir = 25;
	uint64_t score = 0;
	for (int i=0; i<5; i++) {
		printf("%s: ", codes[i]);
		struct String inputs = findNumPadInput(codes[i]);
		printf("checking %s at %d layers\n", inputs.arr, numRobotsWithDir);
		
		uint64_t thisScore = 0;
		for (int i=0; i<inputs.size-1; i++) {
			thisScore += countInputsNeeded(inputs.arr[i], curPos, numRobotsWithDir, memo);
			curPos[numRobotsWithDir] = inputs.arr[i];
		}
		int numCode = strtol(codes[i], NULL, 10);
		printf("num part = %d\n", numCode);
		uint64_t inputLen = thisScore;
		printf("length of input = %lu\n", inputLen);

		score += numCode * inputLen;

		free(inputs.arr);
	}

	printf("score = %lu\n", score);

}
