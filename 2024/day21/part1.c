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

void printBackOneLevel(char *input) {
	int curRow = 0;
	int curCol = 2;
	for (int i=0; i<strlen(input); i++) {
		switch (input[i]) {
		case 'A':
			printf("%c", dirPad[curRow][curCol]);
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

	int moveLeftFirst = startRow > 0;

	if (moveLeftFirst) {
		while (startCol > endCol) {
			addToStr(&moves, '<');
			startCol--;
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

struct String findDirPadInput(char *goal) {
	int curRow = 0;
	int curCol = 2;

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

	int numRobotsWithDir = 2; // this refactor was after seeing part2...
	uint64_t score = 0;
	for (int i=0; i<5; i++) {
		printf("%s: ", codes[i]);
		struct String inputs = findNumPadInput(codes[i]);
		//printf("%s\n", inputs.arr);
		
		struct String thisLevel = findDirPadInput(inputs.arr);
		printf("%s\n", thisLevel.arr);
		for (int j=0; j<numRobotsWithDir-1; j++) {
			struct String nextLevel = findDirPadInput(thisLevel.arr);
			free(thisLevel.arr);
			thisLevel = nextLevel;
			printf("%s\n", thisLevel.arr);
		}
		
		int numCode = strtol(codes[i], NULL, 10);
		printf("num part = %d\n", numCode);
		uint64_t inputLen = thisLevel.size-1;
		printf("length of input = %lu\n", inputLen);

		score += numCode * inputLen;

		free(inputs.arr);
		free(thisLevel.arr);
	}

	printf("score = %lu\n", score);

}
