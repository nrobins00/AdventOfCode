#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Grid {
	char *arr;
	uint32_t *costArr;
	int rows, cols;
};

struct Pos {
	int row, col;
};

enum Dir {
	UP = 0,
	RIGHT,
	DOWN,
	LEFT,
};

void printGrid(struct Grid grid) {
	for (int i=0; i<grid.rows*grid.cols; i++) {
		printf("%c", grid.arr[i]);
		if ((i+1) % grid.cols == 0) {
			printf("\n");
		}
	}
}

int getDictIndex(struct Grid grid, struct Pos pos, enum Dir dir) {
	return (pos.row * grid.cols + pos.col) * 10 + dir;
}

int getTurnCost(enum Dir from, enum Dir to) {
	if (to == from) { return 0; }
	if ((from-to) % 2 == 0) return 2000;
	return 1000;
}

char getCharAt(struct Grid grid, struct Pos start) {
	int index = start.row * grid.cols + start.col;
	return grid.arr[index];
}

int findShortestPath(struct Grid grid, struct Pos start, enum Dir facing) {
	int curIndex = getDictIndex(grid, start, facing);
	uint32_t curCost = grid.costArr[curIndex];

	//printf("pos = (%d, %d); facing = %d; cost = %u\n", start.row, start.col, facing, curCost);

	if (getCharAt(grid, start) == 'E') {
		return curCost;
	}

	int newCost;
	struct Pos newPos;
	int index;
	uint32_t smallestCost = -1;

	for (int i=UP; i<=LEFT; i++){
		newPos.row = start.row;
		newPos.col = start.col;
		switch (i) {
		case UP:
			newPos.row--;
			break;
		case RIGHT:
			newPos.col++;
			break;
		case DOWN:
			newPos.row++;
			break;
		case LEFT:
			newPos.col--;
			break;
		}
		//printf("newPos = (%d, %d); facing = %d\n", newPos.row, newPos.col, i);
		if (getCharAt(grid, newPos) == '#') {
			//printf("hit a wall\n");
			continue;
		}

		newCost = curCost + getTurnCost(facing, i) + 1;
		index = getDictIndex(grid, newPos, i);
		if (grid.costArr[index] > newCost) {
			grid.costArr[index] = newCost;
			// follow this path
			int totalPathCost = findShortestPath(grid, newPos, i);
			if (totalPathCost < smallestCost) {
				smallestCost = totalPathCost;
			}
		}
	}
	return smallestCost;
}

int main(int argc, char**argv) {
	FILE *file = fopen(argv[1], "r");

	char currChar;
	struct Grid grid;
	grid.rows = 0;
	grid.cols = 0;
	while ((currChar = fgetc(file)) != EOF) {
		if (currChar == '\n') {
			grid.rows++;
		} else if (grid.rows == 0) {
			grid.cols++;
		}
	}
	printf("rows = %d; cols = %d\n", grid.rows, grid.cols);

	grid.arr = malloc(grid.rows * grid.cols * sizeof(char));
	uint32_t maxCostArrSize = grid.rows*grid.cols*10+4;
	grid.costArr = malloc(maxCostArrSize * sizeof(uint32_t)); // bigger than necessary
	for (int i=0; i<grid.rows*grid.cols*10 + 4; i++) {
		grid.costArr[i] = -1;
	}
	rewind(file);
	int row = 0;
	int col = 0;
	struct Pos startPos;
	while ((currChar = fgetc(file)) != EOF) {
		switch (currChar) {
		case '\n':
			row++;
			col=0;
			break;
		case 'S':
			startPos.row = row;
			startPos.col = col;
		default:
			grid.arr[row*grid.cols + col++] = currChar;
			break;
		}
	}
	fclose(file);
	printGrid(grid);
	grid.costArr[getDictIndex(grid, startPos, RIGHT)] = 0;
	int shortest = findShortestPath(grid, startPos, RIGHT);
	printf("shortest = %u\n", shortest);

	free(grid.costArr);
	free(grid.arr);
	return 0;
}

