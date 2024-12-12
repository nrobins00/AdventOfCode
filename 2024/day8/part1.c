#include <stdio.h>
#include <stdlib.h>

struct Grid {
    int rowCount;
    int colCount;
    int numAntennas;
    int numAntinodes;
    struct Node *antennas;
    struct Pos *antinodeList;
};

struct Pos {
    int row;
    int col;
};

struct Node {
    char id;
    struct Pos pos;
};


void printGrid(struct Grid grid) {
    printf("numAntennas: %d\n", grid.numAntennas);
    for (int i=0; i<grid.numAntennas; i++) {
        printf("%c antenna at (%d, %d)\n", grid.antennas[i].id,
                grid.antennas[i].pos.row, grid.antennas[i].pos.col);
    }
}

void readGridFromFile(struct Grid *grid, FILE *file) {
    char currChar;
    int rowCount=0;
    int colCount=0;
    int nodeCount=0;
    while ((currChar = fgetc(file)) != EOF) {
        switch (currChar) {
        case '\n':
            rowCount++;
            break;
        case '.':
            if (rowCount==0) {
                colCount++;
            }
            break;
        default:
            nodeCount++;
            if (rowCount==0) {
                colCount++;
            }
        }
    }
    rewind(file);

    struct Node *antens = (struct Node*)malloc(nodeCount*sizeof(struct Node));
    if (!antens) {
        printf("couldn't allocate antens arr\n");
        fclose(file);
        exit(0);
    }

    int row=0;
    int col=0;
    int antenIdx = 0;
    struct Node *newNode;
    while ((currChar = fgetc(file)) != EOF) {
        switch (currChar) {
        case '\n':
            row++;
            col=0;
            break;
        case '.':
            col++;
            break;
        default:
            newNode = (struct Node*)malloc(sizeof(struct Node));
            newNode->pos.row = row;
            newNode->pos.col = col;
            newNode->id = currChar;
            antens[antenIdx++] = *newNode;
            break;
        }
    }

    printf("rowcount: %d, colCount: %d\n", rowCount, colCount);
    grid->rowCount = rowCount;
    grid->colCount = colCount;
    grid->antennas = antens;
    grid->numAntennas = nodeCount;
}

int checkBounds(struct Grid grid, int x, int y) {
    if (x < 0 || x > grid.colCount-1) return 0;
    if (y < 0 || y > grid.rowCount-1) return 0;
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


    free(grid.antennas);
    //free(grid.antinodeList);
}
