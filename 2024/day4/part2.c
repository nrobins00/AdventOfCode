#include <stdio.h>
#include <stdlib.h>

int checkIncline(char **grid, int row, int col) {
	return grid[row+1][col-1] == 'M' && grid[row-1][col+1] =='S'
	 || grid[row+1][col-1] == 'S' && grid[row-1][col+1] == 'M';
}

int checkDecline(char **grid, int row, int col) {
	return grid[row-1][col-1] == 'M' && grid[row+1][col+1] =='S'
	 || grid[row-1][col-1] == 'S' && grid[row+1][col+1] == 'M';

}

int searchForXmas(char **grid, int row, int col, int rowCount, int colCount) {
	int count = 0;
	//int rowCount = sizeof(grid) / sizeof(grid[0]);
	//int colCount = sizeof(grid[0]) / sizeof(grid[0][0]);
	//printf("calculated rowCount: %d\ncalculated colcount: %d\n", rowCount, colCount);
	printf("searching at %d, %d\n", row, col);
	if (row==0 || row==rowCount-1 || col==0 || col==colCount-1) { return 0; }
	count = checkDecline(grid, row, col) && checkIncline(grid, row, col);
	printf("count: %d\n", count);
	return count;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("incorrect argc\n");
	}

	FILE *file = fopen(argv[1], "r");

	char currChar;
	int rowCount = 0;
	int totalChars = 0;
	while ((currChar = fgetc(file)) != EOF) {
		if (currChar == '\n') {
			rowCount++;
		} else {
			totalChars++;
		}
	}
	rewind(file);

	int colCount = totalChars / rowCount;

	printf("charsPerLine: %d\nlineCount: %d\n", colCount, rowCount);

	char **grid = (char**)malloc(rowCount*sizeof(char*));
	if (grid == NULL) {
		printf("failed to allocate grid\n");
		fclose(file);
		exit(0);
	}
	for (int i=0; i<rowCount; i++) {
		grid[i] = (char*)malloc((colCount+1) * sizeof(char));
		if (grid[i] == NULL) {
			printf("failed to allocate string at row %d\n", i);
			for (int j=0; j<i; j++) {
				free(grid[j]);
			}
			fclose(file);
			exit(0);
		}
	}

	for (int i=0; i<rowCount; i++) {
		fgets(grid[i], colCount+1, file);
		grid[i][colCount] = '\0';
		fgetc(file); //ignore newlines
	}

	//for (int i=0; i<rowCount; i++) {
	//	printf("%s\n", grid[i]);
	//}

	int xmasCount=0;
	for (int i=0; i<rowCount; i++) {
		for (int j=0; j<colCount; j++) {
			currChar = grid[i][j];
			if (currChar == 'A') {
				printf("%c", currChar);
				xmasCount += searchForXmas(grid, i, j, rowCount, colCount);

			}
		}
		printf("\n");
	}

	printf("count: %d\n", xmasCount);
	

	for (int i=0; i<rowCount; i++) {
		free(grid[i]);
	}
	free(grid);
	fclose(file);
}
