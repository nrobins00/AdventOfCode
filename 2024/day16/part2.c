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

struct Set {
	struct Pos *arr;
	int cap;
	int size;
};

enum Dir {
	UP = 0,
	RIGHT,
	DOWN,
	LEFT,
};

void addToSet(struct Set *list, struct Pos pos) {
	for (int i=0; i<list->size; i++) {
		struct Pos thisPos = list->arr[i];
		if (thisPos.row == pos.row && thisPos.col == pos.col) {
			return;
		}
	}

	if (list->size == list->cap) {
		list->cap *= 2;
		list->arr = reallocarray(list->arr, list->cap, sizeof(struct Pos));
	}

	list->arr[list->size++] = pos;
}

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

void traverseBackwards(struct Grid grid, struct Pos pos, enum Dir facing, uint32_t curCost, struct Set *visited) {
	printf("at pos (%d, %d) facing %d\n", pos.row, pos.col, facing);

	addToSet(visited, pos);

	if (getCharAt(grid, pos) == 'S') {
		return;
	}

	switch (facing) {
		case UP:
			pos.row++;
			break;
		case RIGHT:
			pos.col--;
			break;
		case DOWN:
			pos.row--;
			break;
		case LEFT:
			pos.col++;
			break;
	}
	curCost--;

	uint32_t newCost;
	int index;
	for (int i=UP; i<=LEFT; i++) {
		newCost = curCost - getTurnCost(facing, i);
		index = getDictIndex(grid, pos, i);
		printf("checking (%d, %d) facing %d\n", pos.row, pos.col, i);
		printf("costArr holds %u; newCost= %u\n", grid.costArr[index], newCost);
		if (grid.costArr[index] == newCost) {
			traverseBackwards(grid, pos, i, newCost, visited);
		}
	}
}


int countGoodSeats(struct Grid grid, uint32_t totalCost) {

	struct Set visited;
	visited.cap = 10;
	visited.size = 0;
	visited.arr = malloc(visited.cap * sizeof(struct Pos));

	struct Pos endPos;
	for (int row=0; row<grid.rows; row++) {
		for (int col=0; col<grid.cols; col++) {
			if (grid.arr[row*grid.cols+col] == 'E') {
				endPos.row = row;
				endPos.col = col;
				break;
			}
		}
	}
	for (int i=UP; i<=LEFT; i++) {
		int index = getDictIndex(grid, endPos, i);
		if (grid.costArr[index] == totalCost) { 
			traverseBackwards(grid, endPos, i, totalCost, &visited);
		}
	}

	return visited.size;

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
	int seats = countGoodSeats(grid, shortest);
	printf("shortest = %u\n", shortest);
	printf("seats = %u\n", seats);

	free(grid.costArr);
	free(grid.arr);
	return 0;
}

