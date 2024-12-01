#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

int comp(const void *el1, const void *el2) {
	int num1 = *((int32_t*)el1);
	int num2 = *((int32_t*)el2);
	return (num1 > num2) - (num2 > num1);
}


int main(int argc, char **argv) {
	if (argc != 2) {
		printf("incorrect argument count\n");
		exit(0);
	}
	FILE *input = fopen(argv[1], "r");
	if (input == NULL) {
		printf("couldn't open file\n");
		exit(0);
	}

	int lineCount = 0;

	char currChar;
	while ((currChar = fgetc(input)) != EOF) {
		if (currChar == '\n') {
			lineCount++;
		}
	}

	rewind(input);
	int32_t *leftList = malloc(lineCount*sizeof(int32_t));
	if (leftList == NULL) {
		printf("left memory not allocated.\n");
		fclose(input);
		exit(0);
	}

	int32_t *rightList = malloc(lineCount*sizeof(int32_t));

	if (rightList == NULL) {
		printf("right memory not allocated.\nFreeing left mem.\n");
		fclose(input);
		free(leftList);
		exit(0);
	}


	int sum = 0;
	int leftNum;
	int rightNum;
	char currString[10];
	int currPos = 0;
	bool onLeftNum = true;
	int currLine = 0;
	while ((currChar = fgetc(input)) != EOF) {
		printf("%c", currChar);
		switch (currChar) {
		case '\n':
			//done with right string
			currString[currPos] = '\0';
			rightNum = atoi(currString);
			//printf("left: %d; right %d\n", leftNum, rightNum);
			currPos = 0;
			onLeftNum = true; // about to start new line
			rightList[currLine] = rightNum;
			currLine++;
			break;
		case ' ':
			// done with left string or just working through 
			if (!onLeftNum) {break;} // 3 consecutive spaces between left and right onLeftNum = false;
			onLeftNum = false;
			currString[currPos] = '\0';
			printf("currString = %s\n", currString);
			leftNum = atoi(currString);
			printf("leftNum = %d\n", leftNum);
			leftList[currLine] = leftNum;
			currPos = 0;
			break;
		default:
			currString[currPos++] = currChar;
			break;
		}
	}

	qsort(leftList, lineCount, sizeof(int32_t), &comp);
	qsort(rightList, lineCount, sizeof(int32_t), &comp);

	for (int i=0; i < lineCount; i++) {
		int right = rightList[i];
		int left = leftList[i];
		printf("left: %d; right: %d\n", left, right);
		if (right > left) {
			sum += right - left;
		} else {
			sum += left - right;
		}
	}

	printf("sum: %d\n", sum);

	free(leftList);
	free(rightList);
}

