#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int indexOfSpace(int *blocks, int totalBlocks, int blocksNeeded) {
	int currSegLen = 0;
	int startOfSeg;
	for (int i=0; i<totalBlocks; i++) {
		if (blocks[i] == -1) {
			if (currSegLen == 0) {
				startOfSeg = i;
			}
			currSegLen++;
			if (currSegLen == blocksNeeded) {
				return startOfSeg;
			}
		} else {
			currSegLen = 0;
		}
	}
	return -1;
}

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

	int *blocks = (int*)malloc(totalBlocks*sizeof(int));
	if (blocks == NULL) {
		printf("couldn't allocate blocks.\n");
		exit(0);
	}
	int blockIndex = 0;
	int currNum;
	int fileIndex = 0;
	int onSpace = 0;
	while ((currChar = fgetc(file)) != EOF) {
		currNum = currChar - 48;
		int stopNum = blockIndex + currNum;
		for (; blockIndex<stopNum; blockIndex++) {
			if (onSpace) {
				blocks[blockIndex] = -1;
			}
			else {
				blocks[blockIndex] = fileIndex;
			}
		}
		if (!onSpace) {
			fileIndex++;
		}
		onSpace = !onSpace;
	}
	for (int i=0; i<totalBlocks; i++) {
		if (blocks[i] == -1) {
			printf(".");
		} else {
			printf("%d", blocks[i]);
		}
	}
	printf("\n");
	// move everything to fill in gaps on left
	for (int i=fileIndex-1; i>0; i--) {
		int endOfFile = 0;
		int startOfFile = 0;
		for (int j=totalBlocks-1; j >=0; j--) {
			if (blocks[j] == i && endOfFile == 0) {
				endOfFile = j;
			}
			if (blocks[j] != i && endOfFile > 0) {
				startOfFile = j+1;
				break;
			}
		}
		printf("file %d range = %d, %d\n", i, startOfFile, endOfFile);
		int sizeOfFile = endOfFile - startOfFile + 1;
		int newLoc = indexOfSpace(blocks, totalBlocks, sizeOfFile);
		if (newLoc > -1 && newLoc < startOfFile) {
			memcpy(&blocks[newLoc], &blocks[startOfFile], sizeOfFile*sizeof(int));
			memset(&blocks[startOfFile], -1, sizeOfFile*sizeof(int));
		}
	}
	for (int i=0; i<totalBlocks; i++) {
		if (blocks[i] == -1) {
			printf(".");
		} else {
			printf("%d", blocks[i]);
		}
	}
	printf("\n");
	// calculate checksum
	uint64_t checksum=0;
	int blockNum;
	for (int i=0; i<totalBlocks; i++) {
		if (blocks[i] == -1) { continue; }
		checksum += i * blocks[i];
	}

	printf("checksum: %lu\n", checksum);

	free(blocks);
	fclose(file);
}
