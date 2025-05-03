#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct Pos {
	int x,y;
};

struct Grid {
	char *arr;
	int rowCount, colCount;
	struct Pos robotPos;
};

struct BoxList {
	struct Pos *arr;
	int size;
	int capacity;
};

void printGrid(struct Grid grid, char robChar) {
	char out[grid.rowCount*grid.colCount+1+grid.rowCount];
	int outIndex=0;
	for (int y=0; y<grid.rowCount; y++) {
		for (int x=0; x<grid.colCount; x++) {
			//printf("printing (%d, %d)\n", x, y);
			int index = y*grid.colCount + x;
			char cur;
			if (x == grid.robotPos.x && y == grid.robotPos.y) {
				cur = robChar;
			} else {
				cur = grid.arr[index];
			}
			if (cur == '.') { cur = ' '; }
			out[outIndex++]=cur;
		}
		out[outIndex++]='\n';
	}
	out[outIndex]='\0';
	printf("%s", out);
}

struct Grid parseGrid(FILE *file) {
	char currChar;
	int rowCount=0;
	int colCount=0;
	while ((currChar = fgetc(file)) != EOF) {
		if (currChar == '\n') {
			rowCount++;
		} else if (rowCount == 0) {
			colCount += 2;
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
			grid.arr[curY*colCount + curX+1] = '.';
			curX += 2;
			break;
		case 'O':
			grid.arr[curY*colCount+curX] = '[';
			grid.arr[curY*colCount+curX+1] = ']';
			curX += 2;
			break;
		default:
			grid.arr[curY*colCount + curX] = currChar;
			grid.arr[curY*colCount + curX+1] = currChar;
			curX += 2;
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

void addToBoxList(struct BoxList *boxList, struct Pos box) {
	if (boxList->size == boxList->capacity) {
		boxList->capacity = boxList->capacity*2;
		boxList->arr = realloc(boxList->arr, boxList->capacity*sizeof(struct Pos));
	}

	boxList->arr[boxList->size++] = box;
}

int canMoveBox(struct Grid grid, struct Pos leftPos, int dx, int dy, struct BoxList *boxList) {
	printf("checking pos (%d, %d)\n", leftPos.x, leftPos.y);
	int xToCheck = leftPos.x + dx;
	int yToCheck = leftPos.y + dy;
	int indexToCheck = yToCheck*grid.colCount+xToCheck;

	char leftChar = grid.arr[indexToCheck];
	char rightChar = grid.arr[indexToCheck+1];

	addToBoxList(boxList, leftPos);

	if (leftChar == '#' || rightChar == '#') {
		return 0;
	}

	if (dx) {
		// moving horizontally means we'll overlap
		if (leftChar == '.' || rightChar == '.') {
			return 1;
		}
		if (dx < 0) {
			leftPos.x -= 2;
			return canMoveBox(grid, leftPos, dx, dy, boxList);
		}
		leftPos.x += 2;
		return canMoveBox(grid, leftPos, dx, dy, boxList);
	} 
	if (leftChar == '.' && rightChar == '.') {
		return 1;
	}
	if (leftChar == '[') {
		leftPos.y += dy;
		return canMoveBox(grid, leftPos, dx, dy, boxList);
	}

	if (leftChar == ']') {
		struct Pos leftPosLeftBox;
		leftPosLeftBox.x = leftPos.x-1;
		leftPosLeftBox.y = leftPos.y + dy;
		if (!canMoveBox(grid, leftPosLeftBox, dx, dy, boxList)) {
			return 0;
		}
	}

	if (rightChar == '[') {
		struct Pos leftPosRightBox;
		leftPosRightBox.x = leftPos.x+1;
		leftPosRightBox.y = leftPos.y + dy;
		return canMoveBox(grid, leftPosRightBox, dx, dy, boxList);
	}
	
	return 1;
}

int comparePointsByYAsc(const void* pointA, const void* pointB) {
	struct Pos a = *((struct Pos *)pointA);
	struct Pos b = *((struct Pos *)pointB);

	return a.y - b.y;
}

int comparePointsByYDesc(const void* pointA, const void* pointB) {
	struct Pos a = *((struct Pos *)pointA);
	struct Pos b = *((struct Pos *)pointB);

	return b.y - a.y;
}

int comparePointsByXAsc(const void* pointA, const void* pointB) {
	struct Pos a = *((struct Pos *)pointA);
	struct Pos b = *((struct Pos *)pointB);

	return a.x - b.x;
}

int comparePointsByXDesc(const void* pointA, const void* pointB) {
	struct Pos a = *((struct Pos *)pointA);
	struct Pos b = *((struct Pos *)pointB);

	return b.x - a.x;
}

struct Grid moveRobot(struct Grid grid, char instruction) {
	printf("moving robot %c\n", instruction);
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

	int newX = grid.robotPos.x + dx;
	int newY = grid.robotPos.y + dy;
	int newIndex = newY*grid.colCount+newX;

	struct Grid newGrid = grid;

	if (grid.arr[newIndex] == '.') {
		newGrid.robotPos.x += dx;
		newGrid.robotPos.y += dy;
		return newGrid;
	}

	if (grid.arr[newIndex] == '#') {
		return grid;
	}

	struct Pos boxLeftPos;
	if (grid.arr[newIndex] == '[') {
		boxLeftPos.x = newX;
		boxLeftPos.y = newY;
	} else if (grid.arr[newIndex] == ']') {
		boxLeftPos.x = newX-1;
		boxLeftPos.y = newY;
	}

	struct BoxList boxList;
	boxList.capacity = 10;
	boxList.size = 0;
	boxList.arr = (struct Pos *)malloc(boxList.capacity*sizeof(struct Pos));
	if (!canMoveBox(grid, boxLeftPos, dx, dy, &boxList)) {
		free(boxList.arr);
		return grid;
	}

	newGrid.robotPos.x += dx;
	newGrid.robotPos.y += dy;

	// sort the box positions by direction so that we don't overwrite
	if (dx > 0) {
		qsort(boxList.arr, boxList.size, sizeof(struct Pos),comparePointsByXAsc);
	} else if (dx < 0) {
		qsort(boxList.arr, boxList.size, sizeof(struct Pos),comparePointsByXDesc);
	} else if (dy > 0) {
		qsort(boxList.arr, boxList.size, sizeof(struct Pos),comparePointsByYAsc);
	} else {
		qsort(boxList.arr, boxList.size, sizeof(struct Pos),comparePointsByYDesc);
	}

	for (int i=boxList.size-1; i>=0; i--) {
		struct Pos boxPos = boxList.arr[i];
		int boxIdx = boxPos.y * grid.colCount + boxPos.x;
		newGrid.arr[boxIdx] = '.';
		newGrid.arr[boxIdx+1] = '.';

		boxPos.x += dx;
		boxPos.y += dy;
		boxIdx = boxPos.y * grid.colCount + boxPos.x;
		newGrid.arr[boxIdx] = '[';
		newGrid.arr[boxIdx+1] = ']';
	}

	free(boxList.arr);
	return newGrid;
}

uint64_t calcScore(struct Grid grid) {
	uint64_t score=0;
	for (int y=0; y<grid.rowCount; y++) {
		for (int x=0; x<grid.colCount; x++) {
			int index = y*grid.colCount + x;
			if (grid.arr[index] != '[') continue;
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
		//printGrid(grid, instructionList[i]);
		//usleep(300*1000);
	}
	uint64_t score = calcScore(grid);
	printf("score = %lu\n", score);

	free(grid.arr);
	fclose (gridFile);
	fclose (instrFile);
}
