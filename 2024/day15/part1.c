#include <stdio.h>
#include <stdlib.h>

struct Pos {
	int x,y;
};

struct Grid {
	char *arr;
	int rowCount, colCount;
	struct Pos robotPos;
};

void printGrid(struct Grid grid) {
	for (int y=0; y<grid.rowCount; y++) {
		for (int x=0; x<grid.colCount; x++) {
			//printf("printing (%d, %d)\n", x, y);
			int index = y*grid.colCount + x;
			char cur;
			if (x == grid.robotPos.x && y == grid.robotPos.y) {
				cur = '@';
			} else {
				cur = grid.arr[index];
			}
			printf("%c", cur);
		}
		printf("\n");
	}
}

struct Grid parseGrid(FILE *file) {
	char currChar;
	int rowCount=0;
	int colCount=0;
	while ((currChar = fgetc(file)) != EOF) {
		if (currChar == '\n') {
			rowCount++;
		} else if (rowCount == 0) {
			colCount++;
		}
	}
	struct Grid grid;
	grid.arr = (char *)malloc(rowCount*colCount*sizeof(char));
	rewind(file);
	int curX = 0;
	int curY = 0;
	while ((currChar = fgetc(file)) != EOF) {
		switch (currChar) {
		case '\n':
			curY++;
			curX = 0;
			break;
		case '@': 
			grid.robotPos.x = curX;
			grid.robotPos.y = curY;
			grid.arr[curY*colCount + curX] = '.';
			curX++;
			break;
		default:
			grid.arr[curY*colCount + curX] = currChar;
			curX++;
			break;
		}
	}
	grid.rowCount = rowCount;
	grid.colCount = colCount;
	return grid;
}

int parseInstructions(FILE *file, char **instructions) {
	char currChar;
	int numInstructions = 0;
	while ((currChar = fgetc(file)) != EOF) {
		if (currChar == '\n') {
			continue;
		} else {
			numInstructions++;
		}
	}
	rewind(file);
	int idx = 0;
	*instructions = (char *)malloc(numInstructions*sizeof(char));
	while ((currChar = fgetc(file)) != EOF) {
		if (currChar == '\n') { continue; }
		(*instructions)[idx++] = currChar;
	}

	return numInstructions;
}

struct Grid moveRobot(struct Grid grid, char instruction) {
	int dx = 0, dy = 0;
	switch (instruction) {
	 case '^':
		dy = -1;
		break;
	case '>':
		dx = 1;
		break;
	case 'v':
		dy = 1;
		break;
	case '<':
		dx = -1;
		break;
	}
	int stopX = grid.robotPos.x;
	int stopY = grid.robotPos.y;
	int done = 0;
	while (!done) {
		stopX += dx;
		stopY += dy;
		int index = stopY*grid.colCount+stopX;
		if (grid.arr[index] == '.') {
			done=1;
		} else if (grid.arr[index] == '#') {
			done=2;
		}
	}
	if (done==2) {
		// hit a wall
		return grid;
	}
	struct Grid newGrid = grid;
	newGrid.robotPos.x += dx;
	newGrid.robotPos.y += dy;

	int curX = newGrid.robotPos.x;
	int curY = newGrid.robotPos.y;
	newGrid.arr[curY*newGrid.colCount+curX] = '.';
	done = curX == stopX && curY == stopY;
	while (!done) {
		curX += dx;
		curY += dy;
		int index = curY * newGrid.colCount + curX;
		newGrid.arr[index] = 'O';
		done = curX == stopX && curY == stopY;
	}
	return newGrid;
}

int calcScore(struct Grid grid) {
	int score=0;
	for (int y=0; y<grid.rowCount; y++) {
		for (int x=0; x<grid.colCount; x++) {
			int index = y*grid.colCount + x;
			if (grid.arr[index] != 'O') continue;
			score += 100*y + x;
		}
	}
	return score;
}


int main(int argc, char **argv) {
	FILE *gridFile = fopen(argv[1], "r");
	FILE *instrFile = fopen(argv[2], "r");

	struct Grid grid = parseGrid(gridFile);
	char *instructionList;
	int numInstructions = parseInstructions(instrFile, &instructionList);

	//printGrid(grid);
	for (int i=0; i<numInstructions; i++) {
		grid = moveRobot(grid, instructionList[i]);
		//printGrid(grid);
	}
	int score = calcScore(grid);
	printf("score = %d\n", score);

	free(grid.arr);
	fclose (gridFile);
	fclose (instrFile);
}
