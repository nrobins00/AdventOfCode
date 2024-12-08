#include <stdio.h>
#include <stdlib.h>

struct Grid {
    int rowCount;
    int colCount;
    char **arr;
    int posX, posY;
    char direction;
};

void printGrid(struct Grid grid) {
    for (int i=0; i<grid.rowCount; i++) {
        for (int j=0; j<grid.colCount; j++) {
            printf("%c", grid.arr[i][j]);
        }
        printf("\n");
    }
}

void readGridFromFile(struct Grid *grid, FILE *file) {
    char currChar;
    int rowCount=0;
    int colCount=0;
    while ((currChar = fgetc(file)) != EOF) {
        switch (currChar) {
        case '\n':
            rowCount++;
            break;
        default:
            if (rowCount==0) {
                colCount++;
            }
        }
    }
    rewind(file);

    char **arr = (char**)malloc(rowCount*sizeof(char*));
    if (!arr) return;
    for (int i=0; i<rowCount; i++) {
        arr[i] = (char*)malloc(colCount*sizeof(char));
        if (!arr[i]) {
            for (int j=0; j<i; j++) {
                free(arr[j]);
            }
            return;
        }
    }

    int row = 0;
    int col=0;
    while ((currChar = fgetc(file)) != EOF) {
        switch (currChar) {
        case '\n':
            row++;
            col=0;
            break;
        case '^':
        case '>':
        case 'v':
        case '<':
            grid->posX = col;
            grid->posY = row;
            grid->direction = currChar;
        default:
            arr[row][col++] = currChar;
            break;
        }
    }

    grid->rowCount = rowCount;
    grid->colCount = colCount;
    grid->arr = arr;
}

char rotateDirection(char direction) {
    switch (direction) {
    case '^':
        return '>';
    case '>':
        return 'v';
    case 'v':
        return '<';
    case '<':
        return '^';
    }

    printf("wtf?");
    exit(0);
}

int checkBounds(struct Grid grid, int x, int y) {
    if (x < 0 || x > grid.colCount-1) return 0;
    if (y < 0 || y > grid.rowCount-1) return 0;
    return 1;
}

int incrementPos(struct Grid *grid) {
    grid->arr[grid->posY][grid->posX] = 'X';
    int newX = grid->posX;
    int newY = grid->posY;
    switch (grid->direction) {
    case '^':
        newY--;
        break;
    case '>':
        newX++;
        break;
    case 'v':
        newY++;
        break;
    case '<':
        newX--;
        break;
    }
    printf("newX: %d; newY: %d; dir: %c\n", newX, newY, grid->direction);
    if (!checkBounds(*grid, newX, newY)) return 0;

    if (grid->arr[newY][newX] == '#') {
        grid->direction = rotateDirection(grid->direction);
        return incrementPos(grid);
    }

    grid->posX = newX;
    grid->posY = newY;
    return 1;
}



int main(int argc, char** argv) {
	if (argc != 2) { 
		printf("incorrect arg count\n");
		exit(0);
	}

	FILE *inputFile = fopen(argv[1], "r");
    struct Grid grid;
    readGridFromFile(&grid, inputFile);
    fclose(inputFile);

    printGrid(grid);

    while (incrementPos(&grid)) {
    }

    int visitedCount = 0;
    for (int i=0; i<grid.rowCount; i++) {
        for (int j=0; j<grid.colCount; j++) {
            if (grid.arr[i][j] == 'X') {
                visitedCount++;
            }
        }
    }

    printf("visitedCount: %d\n", visitedCount);
    //printGrid(grid);

    for (int row=0; row<grid.rowCount; row++) {
        free(grid.arr[row]);
    }
    free(grid.arr);
}
