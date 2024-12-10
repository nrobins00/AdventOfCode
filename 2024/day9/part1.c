#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("incorrect arg count\n");
		exit(0);
	}
	FILE *file = fopen(argv[1], "r");

	char currChar;
	int totalBlocks = 0;
	char currStr[2];
	while ((currChar = fgetc(file)) != EOF) {
		sprintf(currStr, "%c", currChar);
		totalBlocks += atoi(currStr);
	}
	rewind(file);

	char *blocks = (char*)malloc(totalBlocks*sizeof(char) + 1);
	int blockIndex = 0;
	int currNum;
	int fileIndex = 0;
	int onSpace = 0;
	while ((currChar = fgetc(file)) != EOF) {
		sprintf(currStr, "%c", currChar);
		currNum = atoi(currStr);
		int stopNum = blockIndex + currNum;
		printf("blockIndex: %d; stopNum: %d\n", blockIndex, stopNum);
		for (; blockIndex<stopNum; blockIndex++) {
			if (onSpace) {
				blocks[blockIndex] = '.';
			}
			else {
				blocks[blockIndex] = fileIndex;
			}
		}
		onSpace = !onSpace;
		if (!onSpace) {
			fileIndex++;
		}
	}
	blocks[totalBlocks] = '\0';

	printf("%s\n", blocks);

	free(blocks);
	fclose(file);
}
