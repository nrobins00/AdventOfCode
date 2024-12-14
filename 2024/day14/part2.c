#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int rows = 103;
int cols = 101;

int mod(int lhs, int rhs) {
	if (lhs >= 0) {
		return lhs % rhs;
	}
	while (lhs < 0) {
		lhs += rhs;
	}
	return lhs;
}

struct Robot {
	int x, y;
	int dx, dy;
};

struct Grid {
	struct Robot *robots;
	uint16_t robotCount;
};

int isNumeric(char c) {
	return c >= 48 && c <= 57;
}

int parseInt(FILE *file) {
	char currString[12]; // min value for int is -2147483648
	int currStringIx = 0;

	char currChar = fgetc(file);
	while (isNumeric(currChar) || currChar == '-' && currStringIx == 0) {
		currString[currStringIx++] = currChar;
		currChar = fgetc(file);
	}
	ungetc(currChar, file);
	currString[currStringIx] = '\0';

	int ret = atoi(currString);
	return ret;
}


void parseRobots(FILE *file, struct Robot *robotArr, int *countsGrid) {
	char currChar;
	int roboIdx = 0;
	struct Robot robot = { 0, 0, 0, 0 };
	while ((currChar = fgetc(file)) != EOF) {
		switch (currChar ) {
		case '\n':
			// next robot
			robotArr[roboIdx++] = robot;
			countsGrid[robot.y*cols+robot.x]+=1;
			break;
		case 'p':
			fgetc(file); // '='
			robot.x = parseInt(file);
			fgetc(file); // ','
			robot.y = parseInt(file);
			break;
		case 'v':
			fgetc(file); // '='
			robot.dx = parseInt(file);
			fgetc(file); // ','
			robot.dy = parseInt(file);
			break;
		default:
			break;

		}
	}
}

void moveRobot(struct Robot *robot, int numMoves, int *countsGrid) {
	countsGrid[robot->y*cols+robot->x]--;
	int newX = robot->x + robot->dx * numMoves;
	int newY = robot->y + robot->dy * numMoves;

	newX = mod(newX, cols);
	newY = mod(newY, rows);

	robot->x = newX;
	robot->y = newY;
	countsGrid[robot->y*cols+robot->x]++;
}

void printGrid(int *countsGrid) {
	for (int y=0; y<rows; y++) {
		for (int x=0; x<cols; x++) {
			
			int count = countsGrid[y*cols+x];
			if (count == 0) {
				printf(" ");
			} else {
				printf("%d", count);
			}
		}
		printf("\n");
	}
}


int main(int argc, char **argv) {
	FILE *file = fopen(argv[1], "r");

	char currChar;
	int roboCount = 0;
	while ((currChar = fgetc(file)) != EOF) {
		if (currChar == '\n') {
			roboCount++;
		}
	}
	rewind(file);
	struct Robot *robotArr = (struct Robot *)malloc(roboCount * sizeof(struct Robot));

	int *countsGrid = (int *)calloc(rows*cols, sizeof(int));
	parseRobots(file, robotArr, countsGrid);
	printGrid(countsGrid);
	
	//int numMoves = atoi(argv[2]);
	int moveCount = 0;
	if (argc > 2) {
		moveCount = atoi(argv[2]);
		for (int i=0; i<roboCount; i++) {
			moveRobot(&robotArr[i], moveCount, countsGrid);
		}
	}


	while (1) {
		moveCount++;
		for (int i=0; i<roboCount; i++) {
			moveRobot(&robotArr[i], 1, countsGrid);
		}
		if (moveCount % 103 == 72) {
			printf("\033[2J");
			printf("\033[H");
			printGrid(countsGrid);
			printf("numMoves: %d\n", moveCount);
			printf("\n\n\n\n");
			usleep(600*1000); //half a sec
		}
	}
	//int middleY = (rows-1)/2;
	//int middleX = (cols-1)/2;

	//// top left
	//int topLeftCount = 0;
	//int topRightCount = 0;
	//int botLeftCount = 0;
	//int botRightCount = 0;

	//for (int roboIdx=0; roboIdx<roboCount; roboIdx++) {
	//	int x = robotArr[roboIdx].x;
	//	int y = robotArr[roboIdx].y;
	//	if (x < middleX && y < middleY) {
	//		topLeftCount++;
	//	}
	//	if (x > middleX && y < middleY) {
	//		topRightCount++;
	//	}
	//	if (x < middleX && y > middleY) {
	//		botLeftCount++;
	//	}
	//	if (x > middleX && y > middleY) {
	//		botRightCount++;
	//	}
	//}

	//uint64_t sum = topLeftCount*topRightCount*botLeftCount*botRightCount;
	//printf("sum = %lu\n", sum);

	free(robotArr);
	free(countsGrid);
	
	fclose(file);
}
