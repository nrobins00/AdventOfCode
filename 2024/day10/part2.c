#include <stdio.h>
#include <stdlib.h>

struct Point {
	int row, col;
};

struct Map {
	char *grid;
	int charCount;
	int rowSize;
};

char getCharAt(struct Map map, struct Point point) {
	return map.grid[point.row*map.rowSize+point.col];
}

int checkPath(struct Map map, struct Point start) {
	if (start.row >= map.charCount / map.rowSize || start.row < 0) {
		return 0;
	}
	if (start.col >= map.rowSize || start.col < 0) {
		return 0;
	}
	char curVal = getCharAt(map, start);
	if (curVal == '9') {
		return 1;
	}

	int score = 0;
	// check up
	struct Point pointToCheck;
	pointToCheck.row = start.row-1;
	pointToCheck.col = start.col;
	if (getCharAt(map, pointToCheck) == curVal+1) {
		score += checkPath(map, pointToCheck);
	}
	//
	// check right
	pointToCheck.row = start.row;
	pointToCheck.col = start.col+1;
	if (getCharAt(map, pointToCheck) == curVal+1) {
		score += checkPath(map, pointToCheck);
	}
	//
	// down
	pointToCheck.row = start.row+1;
	pointToCheck.col = start.col;
	if (getCharAt(map, pointToCheck) == curVal+1) {
		score += checkPath(map, pointToCheck);
	}
	//
	// left
	pointToCheck.row = start.row;
	pointToCheck.col = start.col-1;
	if (getCharAt(map, pointToCheck) == curVal+1) {
		score += checkPath(map, pointToCheck);
	}

	return score;
}

int getScore(struct Map map, struct Point trHead) {
	int score = checkPath(map, trHead);
	return score;
}


int main(int argc, char **argv) {
	FILE *file = fopen(argv[1], "r");

	char currChar;
	struct Map map;
	int charCount = 0;
	int rowSize = 0;
	int trailHeadCount = 0;
	while ((currChar = fgetc(file)) != EOF) {
		switch (currChar) {
		case '\n':
			if (rowSize == 0) {
				rowSize = charCount;
			}
			break;
		case '0':
			trailHeadCount++;
		default:
			charCount++;
			break;
		}
	}

	char *grid = malloc(charCount * sizeof(char));
	struct Point *trailHeads = malloc(trailHeadCount * sizeof(struct Point));
	map.charCount = charCount;
	map.rowSize = rowSize;
	map.grid = grid;

	rewind(file);
	int row = 0;
	int col = 0;
	int trailHeadIdx = 0;
	while ((currChar = fgetc(file)) != EOF) {
		switch (currChar) {
		case '\n':
			col = 0;
			row++;
			break;
		case '0':
			trailHeads[trailHeadIdx].row = row;
			trailHeads[trailHeadIdx].col = col;
			trailHeadIdx++;
		default:
			grid[row*rowSize+col] = currChar;
			col++;
			break;
		}
	}

	int totalSum = 0;
	for (int headIdx=0; headIdx<trailHeadCount; headIdx++) {
		int sum = getScore(map, trailHeads[headIdx]);
		printf("sum at %d: %d\n", headIdx, sum);
		totalSum += sum;

	}
	printf("sum: %d\n", totalSum);

	free(grid);
	free(trailHeads);
	fclose(file);
}
