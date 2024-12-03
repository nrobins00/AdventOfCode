#include <stdio.h>
#include <stdlib.h>

int verifySafety(int *increasing, int diff) {
	if (diff == 0 || diff > 3 || diff < -3) return 0;
	if (*increasing == 0) {
		*increasing = diff;
		return 1;
	}
	if (*increasing < 0 && diff > 0) { return 0; }
	if (*increasing > 0 && diff < 0) { return 0; }
	return 1;
}

int main(int argc, char** argv) {
	if (argc != 2) { 
		printf("incorrect arg count\n");
		exit(0);
	}
	FILE *inputFile = fopen(argv[1], "r");

	char currChar;
	int isSafe = 1;
	int safeCount = 0;
	int increasing = 0;
	int diff;
	char currString[10];
	int currStringIndex = 0;
	int lastInt = -1;
	int currInt;
	while ((currChar = fgetc(inputFile)) != EOF) {
		switch (currChar) {
		case '\n':
			if (isSafe) {
				currString[currStringIndex] = '\0';
				currInt = atoi(currString);
				diff = currInt - lastInt;
				isSafe = verifySafety(&increasing, diff);
			}

			safeCount += isSafe;
			isSafe = 1;
			lastInt = -1;
			increasing = 0;
			currStringIndex = 0;
			break;
		case ' ':
			if (!isSafe) break;
			currString[currStringIndex] = '\0';
			currStringIndex = 0;

			currInt = atoi(currString);
			if (lastInt >= 0) {
				diff = currInt - lastInt;
				isSafe = verifySafety(&increasing, diff);
			}
			lastInt = currInt;
			break;
		default:
			if (!isSafe) break;
			currString[currStringIndex++] = currChar;
			break;
		}
	}

	fclose(inputFile);
	printf("total safe: %d\n", safeCount);
}
