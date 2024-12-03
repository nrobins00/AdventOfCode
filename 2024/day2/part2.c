#include <stdio.h>
#include <stdlib.h>

void copyEverythingExcept(int *src, int *dest, int exceptIndex) {
	printf("copy everything called\n");
	int i=0;
	int j=0;
	for (;src[i] != -1; i++ ) {
		if (i == exceptIndex) {continue;}
		dest[j++] = src[i];
	}
	dest[j] = -1;
	for (int i=0; dest[i] != -1; i++) {
		printf("%d ", dest[i]);
	}
	printf("\n");
}

int verifySafety(int *report, int recurse) {
	int tryRemovingCurr;
	int tryRemovingLast;
	int tryRemovingSecondLast;
	int removeLevel1[25];
	int removeLevel2[25];
	int removeLevel3[25];
	int lastDiff = 0;
	for (int i=1; report[i] != -1; i++) {
		int diff = report[i] - report[i-1];
		if (diff > 3 || diff < -3 || diff == 0) { 
			if (!recurse) { return 0; }
			copyEverythingExcept(report, removeLevel1, i);
			tryRemovingCurr = verifySafety(removeLevel1, 0);
			copyEverythingExcept(report, removeLevel2, i-1);
			tryRemovingLast = verifySafety(removeLevel2, 0);
			if (i == 2) {
				copyEverythingExcept(report, removeLevel3, i-2);
				tryRemovingSecondLast = verifySafety(removeLevel3, 0);
			} else { tryRemovingSecondLast = 0; }
			return tryRemovingCurr || tryRemovingLast || tryRemovingSecondLast;
		}
		if (lastDiff == 0) { 
			lastDiff = diff;
			continue; 
		}
		if ((lastDiff<0 && diff>0) || (lastDiff>0 && diff<0)) { 
			if (!recurse) { return 0; }
			copyEverythingExcept(report, removeLevel1, i);
			tryRemovingCurr = verifySafety(removeLevel1, 0);
			copyEverythingExcept(report, removeLevel2, i-1);
			tryRemovingLast = verifySafety(removeLevel2, 0);
			if (i == 2) {
				copyEverythingExcept(report, removeLevel3, i-2);
				tryRemovingSecondLast = verifySafety(removeLevel3, 0);
			} else { tryRemovingSecondLast = 0; }
			return tryRemovingCurr || tryRemovingLast || tryRemovingSecondLast;
		}
		lastDiff = diff;
	}
	for (int i=0; report[i] != -1; i++) {
		printf("%d ", report[i]);
	}
	printf("\n");
	return 1;
}

int main(int argc, char** argv) {
	if (argc != 2) { 
		printf("incorrect arg count\n");
		exit(0);
	}
	FILE *inputFile = fopen(argv[1], "r");

	
	int *report = malloc(25*sizeof(int));
	int currLevelIndex = 0;
	char currChar;
	int safeCount = 0;
	char currString[10];
	int currStringIndex = 0;
	int currInt;
	while ((currChar = fgetc(inputFile)) != EOF) {
		switch (currChar) {
		case '\n':
			currString[currStringIndex] = '\0';
			currStringIndex = 0;
			currInt = atoi(currString);

			report[currLevelIndex++] = currInt;
			report[currLevelIndex] = -1; // terminator
			printf("calling verifySafety\n");
			safeCount += verifySafety(report, 1);
			currLevelIndex = 0;
			break;
		case ' ':
			currString[currStringIndex] = '\0';
			currStringIndex = 0;
			currInt = atoi(currString);
			report[currLevelIndex++] = currInt;
			break;
		default:
			currString[currStringIndex++] = currChar;
			break;
		}
	}

	fclose(inputFile);
	printf("total safe: %d\n", safeCount);
}
