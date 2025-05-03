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
    struct Node newNode;
    struct Pos newPos;
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
            newPos.row = row;
            newPos.col = col++;
            newNode.pos = newPos;
            newNode.id = currChar;
            antens[antenIdx++] = newNode;
            break;
        }
    }

    printf("rowcount: %d, colCount: %d\n", rowCount, colCount);
    grid->rowCount = rowCount;
    grid->colCount = colCount;
    grid->antennas = antens;
    grid->numAntennas = nodeCount;
    grid->antinodeList = (struct Pos*)malloc(nodeCount*2*sizeof(struct Pos));
    grid->numAntinodes = 0;
}

int checkBounds(struct Grid grid, struct Pos pos) {
    if (pos.row < 0 || pos.row > grid.rowCount-1) return 0;
    if (pos.col < 0 || pos.col > grid.colCount-1) return 0;
    return 1;
}

int addAntinodeToList(struct Pos *antinodeList, int listSize, struct Pos nodeToAdd) {
    // check for already there
    struct Pos curPos;
    for (int i=0; i<listSize; i++) {
        curPos = antinodeList[i];
        if (curPos.row == nodeToAdd.row && curPos.col == nodeToAdd.col) {
            printf("duplicate found at (%d, %d)\n", curPos.row, curPos.col);
            return 0;
        }
    }
    // add
    antinodeList[listSize] = nodeToAdd;
    return 1;
}

void checkForAntinodes(struct Grid *grid, struct Node node1, struct Node node2) {
    if (node1.id != node2.id) return;
    printf("checking %c (%d, %d) and (%d, %d)\n", node1.id, node1.pos.row, node1.pos.col,
           node2.pos.row, node2.pos.col);
    struct Pos node3;
    int rise, run;
    rise = node2.pos.row - node1.pos.row;
    run = node2.pos.col - node1.pos.col;

    // check both directions
    int numFound = 0;
    // node2 + rise/run
    node3.row = node2.pos.row + rise;
    node3.col = node2.pos.col + run;

    if (checkBounds(*grid, node3)) {
        printf("found antinode at (%d, %d)\n", node3.row, node3.col);
        grid->numAntinodes += addAntinodeToList(grid->antinodeList, grid->numAntinodes, node3);
    }
    //
    // node1 - rise/run
    node3.row = node1.pos.row - rise;
    node3.col = node1.pos.col - run;
    if (checkBounds(*grid, node3)) {
        printf("found antinode at (%d, %d)\n", node3.row, node3.col);
        grid->numAntinodes += addAntinodeToList(grid->antinodeList, grid->numAntinodes, node3);
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

    printGrid(grid);
    for (int i=1; i<grid.numAntennas; i++) {
        for (int j=0; j<i; j++) {
            checkForAntinodes(&grid, grid.antennas[i], grid.antennas[j]);
        }
    }

    printf("antinodes found: %d\n", grid.numAntinodes);

    free(grid.antennas);
    //free(grid.antinodeList);
}
