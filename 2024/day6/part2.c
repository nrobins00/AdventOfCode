#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Grid {
    int rowCount;
    int colCount;
    char **arr;
    int **visitedArr;
    int posCol, posRow;
    char direction;
};

void printGrid(struct Grid grid) {
    for (int i=0; i<grid.rowCount; i++) {
        for (int j=0; j<grid.colCount; j++) {
            printf("%c", grid.arr[i][j]);
        }
        printf("\n");
    }

    for (int i=0; i<grid.rowCount; i++) {
        for (int j=0; j<grid.colCount; j++) {
          printf("%2d,", grid.visitedArr[i][j]);
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

    int **visitedArr = (int**)malloc(rowCount*sizeof(int*));
    for (int i=0; i<rowCount; i++) {
        visitedArr[i] = (int*)calloc(colCount, sizeof(int)); //initialize to 0
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
            grid->posCol= col;
            grid->posRow = row;
            grid->direction = currChar;
            arr[row][col++] = '.';
            break;
        default:
            arr[row][col++] = currChar;
            break;
        }
    }

    grid->rowCount = rowCount;
    grid->colCount = colCount;
    grid->arr = arr;
    grid->visitedArr = visitedArr;
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

int checkForLoop(struct Grid *grid) {
    int currRow = grid->posRow;
    int currCol = grid->posCol;
    int currVisitedVal = grid->visitedArr[currRow][currCol];
    char direction = grid->direction;
    int directionVal;
    

    switch (direction) {
    case '^':
        directionVal = 1 << 0;
        break;
    case '>':
        directionVal = 1 << 1;
        break;
    case 'v':
        directionVal = 1 << 2;
        break;
    case '<':
        directionVal = 1 << 3;
        break;
    }

    if (currVisitedVal & directionVal) {
        printf("identified loop; currVisitedVal: %d, directionVal: %d\n", currVisitedVal, directionVal);
        printf("position: %d, %d\n", grid->posRow, grid->posCol);
        return 1;
    }

    grid->visitedArr[currRow][currCol] = currVisitedVal | directionVal;
    return 0;
}

int incrementPos(struct Grid *grid) {
    int newCol = grid->posCol;
    int newRow = grid->posRow;
    switch (grid->direction) {
    case '^':
        newRow--;
        break;
    case '>':
        newCol++;
        break;
    case 'v':
        newRow++;
        break;
    case '<':
        newCol--;
        break;
    }
    if (!checkBounds(*grid, newCol, newRow)) return 0;

    if (grid->arr[newRow][newCol] == '#') {
        grid->direction = rotateDirection(grid->direction);
        return 1;
    }

    grid->posCol = newCol;
    grid->posRow = newRow;
    return 1;
}

void copyGrid(struct Grid *src, struct Grid *dest, int allocate) {
    dest->rowCount = src->rowCount;
    dest->colCount = src->colCount;
    dest->posRow = src->posRow;
    dest->posCol = src->posCol;
    dest->direction = src->direction;

    if (allocate) {
        dest->arr = (char**)malloc(src->rowCount * sizeof(char*));
        for (int i=0; i<src->rowCount; i++) {
            dest->arr[i] = (char*)malloc(src->colCount * sizeof(char));
        }

        dest->visitedArr = (int**)malloc(src->rowCount * sizeof(int*));
        for (int i=0; i<src->rowCount; i++) {
            dest->visitedArr[i] = (int*)calloc(src->colCount, sizeof(int));
        }
    }

    for (int i=0; i<src->rowCount; i++) {
        memcpy(dest->arr[i], src->arr[i], src->colCount*sizeof(char));
        memcpy(dest->visitedArr[i], src->visitedArr[i], src->colCount*sizeof(int));
    }
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

    struct Grid testGrid;
    copyGrid(&grid, &testGrid, 1);

    int possibleBlockers = 0;
    for (int row=0; row<grid.rowCount; row++) {
        for (int col=0; col<grid.colCount; col++) {
            if (grid.arr[row][col] != '#' && !(row == grid.posRow && row == grid.posCol)) {
                copyGrid(&grid, &testGrid, 0);
                testGrid.arr[row][col] = '#';
                while (1) {
                    if (checkForLoop(&testGrid)) {
                        printf("possibility at %d, %d\n", row, col);
                        possibleBlockers++;
                        //exit(0);
                        break;
                    }
                    if (!incrementPos(&testGrid)) {
                        break;
                    }
                }
            }
        }
    }

    printf("blockers: %d\n", possibleBlockers);
    //printGrid(grid);

    for (int row=0; row<grid.rowCount; row++) {
        free(grid.arr[row]);
    }
    free(grid.arr);
}
