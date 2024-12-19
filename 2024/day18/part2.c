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

uint16_t wasVisited(uint16_t *grid, struct Pos pos) {
	return grid[pos.row*cols + pos.col];
}

uint16_t traverse(char *grid, uint16_t *visited, struct Pos curPos) {
	if (curPos.col == cols-1 && curPos.row == rows-1) {
		return 1;
	}

	struct Pos nextPos;
	//UP
	nextPos.row = curPos.row-1;
	nextPos.col = curPos.col;
	if (getCharAt(grid, nextPos) == '.') {
		if (!wasVisited(visited, nextPos)) {
			//printf("went up\n");
			visited[nextPos.row*cols+nextPos.col] = 1;
			if (traverse(grid, visited, nextPos)) {
				return 1;
			}
		}
	}
	//RIGHT
	nextPos.row = curPos.row;
	nextPos.col = curPos.col+1;
	if (getCharAt(grid, nextPos) == '.') {
		if (!wasVisited(visited, nextPos)) {
			//printf("went up\n");
			visited[nextPos.row*cols+nextPos.col] = 1;
			if (traverse(grid, visited, nextPos)) {
				return 1;
			}
		}
	}
	//DOWN
	nextPos.row = curPos.row+1;
	nextPos.col = curPos.col;
	if (getCharAt(grid, nextPos) == '.') {
		if (!wasVisited(visited, nextPos)) {
			//printf("went up\n");
			visited[nextPos.row*cols+nextPos.col] = 1;
			if (traverse(grid, visited, nextPos)) {
				return 1;
			}
		}
	}
	//LEFT
	nextPos.row = curPos.row;
	nextPos.col = curPos.col-1;
	if (getCharAt(grid, nextPos) == '.') {
		if (!wasVisited(visited, nextPos)) {
			//printf("went up\n");
			visited[nextPos.row*cols+nextPos.col] = 1;
			if (traverse(grid, visited, nextPos)) {
				return 1;
			}
		}
	}
	return 0;
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

	int lastLeftNum = 0;
	int rightNum;
	struct Pos start = {0, 0};
	for (int dropped = count+1; (leftNum = parseNum(file)) >= 0; dropped++) {
		rightNum = parseNum(file);
		grid[rightNum*cols + leftNum] = '#';
		uint16_t *visited = malloc(rows*cols*sizeof(uint16_t));
		visited[0] = 1;
		for (int i=1; i<rows*cols; i++) {
			visited[i] = 0;
		}

		if (!traverse(grid, visited, start)) {
			break;
		}
	}
	printf("stopped at %d,%d\n", leftNum, rightNum);
	fclose(file);
}
