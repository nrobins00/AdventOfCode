#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int rows = 71;
int cols = 71;


struct Pos {
	int row, col;
};

struct Node {
	struct Pos pos;
	struct Node* next;
};

int isNumeric(char c) {
	return c >= 48 && c <= 57;
}

int parseNum(FILE *file) {
	char currString[30];
	int index = 0;
	int currChar;
	while (isNumeric(currChar = fgetc(file))) {
		currString[index++] = currChar;
	}
	if (index == 0) {
		return -1;
	}
	currString[index] = '\0';
	int num = atoi(currString);
	return num;
}

//void addToList(struct Node *list, struct Node toAdd) {
//	struct Node *prev = NULL;
//	while (list->next != NULL && list->pos.dist < toAdd.pos.dist) {
//		prev = list;
//		list = list->next;
//	}
//	if (prev != NULL) {
//		prev->next = &toAdd;
//	}
//
//	toAdd.next = list;
//}
//
char getCharAt(char *grid, struct Pos pos) {
	if (pos.row < 0 || pos.row >= rows || pos.col < 0 || pos.col >= cols) {
		return '#';
	}
	return grid[pos.row*cols + pos.col];
}

uint16_t getCostAt(uint16_t *grid, struct Pos pos) {
	return grid[pos.row*cols + pos.col];
}

uint16_t traverse(char *grid, uint16_t *costGrid, struct Pos curPos) {
	if (curPos.col == cols-1 && curPos.row == rows-1) {
		return getCostAt(costGrid, curPos);
	}

	struct Pos nextPos;
	uint16_t curCost = getCostAt(costGrid, curPos);
	//UP
	uint16_t smallestCost = -1;
	nextPos.row = curPos.row-1;
	nextPos.col = curPos.col;
	if (getCharAt(grid, nextPos) == '.') {
		if (curCost+1 < getCostAt(costGrid, nextPos)) {
			//printf("went up\n");
			costGrid[nextPos.row*cols+nextPos.col] = curCost+1;
			smallestCost = traverse(grid, costGrid, nextPos);
		}
	}
	//RIGHT
	nextPos.row = curPos.row;
	nextPos.col = curPos.col+1;
	if (getCharAt(grid, nextPos) == '.') {
		if (curCost+1 < getCostAt(costGrid, nextPos)) {
			costGrid[nextPos.row*cols+nextPos.col] = curCost+1;
			uint16_t cost = traverse(grid, costGrid, nextPos);
			if (cost < smallestCost) {
				smallestCost = cost;
			}
		}
	}
	//DOWN
	nextPos.row = curPos.row+1;
	nextPos.col = curPos.col;
	if (getCharAt(grid, nextPos) == '.') {
		if (curCost+1 < getCostAt(costGrid, nextPos)) {
			costGrid[nextPos.row*cols+nextPos.col] = curCost+1;
			uint16_t cost = traverse(grid, costGrid, nextPos);
			if (cost < smallestCost) {
				smallestCost = cost;
			}
		}
	}
	//LEFT
	nextPos.row = curPos.row;
	nextPos.col = curPos.col-1;
	if (getCharAt(grid, nextPos) == '.') {
		if (curCost+1 < getCostAt(costGrid, nextPos)) {
			costGrid[nextPos.row*cols+nextPos.col] = curCost+1;
			uint16_t cost = traverse(grid, costGrid, nextPos);
			if (cost < smallestCost) {
				smallestCost = cost;
			}
		}
	}
	return smallestCost;
}

int main(int argc, char **argv) {
	FILE *file = fopen(argv[1], "r");

	char *grid = malloc(rows*cols*sizeof(char));
	for (int i=0; i<rows*cols; i++) {
		grid[i] = '.';
	}
	char currChar;
	int leftNum;
	int count = 0;
	while ((leftNum = parseNum(file)) >= 0) {
		int rightNum = parseNum(file);
		grid[rightNum*cols + leftNum] = '#';
		count++;
		if (count == 1024) {
			break;
		}
	}
	fclose(file);
	for (int i=0; i<rows*cols; i++) {
		if (i % cols == 0) {
			printf("\n");
		}
		printf("%c", grid[i]);
	}

	struct Pos start = {0, 0};
	uint16_t *costGrid = malloc(rows*cols*sizeof(uint16_t));
	costGrid[0] = 0;
	for (int i=1; i<rows*cols; i++) {
		costGrid[i] = -1;
	}
	printf("cost = %d\n", traverse(grid, costGrid, start));
}
