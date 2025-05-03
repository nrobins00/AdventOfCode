#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Instructions {
  int *arr;
  int size;
};

int isNumeric(char c) { return c >= 48 && c <= 57; }

int parseNum(FILE *file) {
  char currString[12];
  int index = 0;
  int currChar;
  while (isNumeric(currChar = fgetc(file))) {
    currString[index++] = currChar;
  }
  currString[index] = '\0';
  int num = atoi(currString);
  return num;
}

int parseInstructions(FILE *file, int *dest) {
  char currChar;
  int instructionCount = 0;
  char num[2];
  while ((currChar = fgetc(file)) != EOF) {
    if (currChar != ',' && currChar != '\n') {
      num[0] = currChar;
      num[1] = '\0';
      dest[instructionCount++] = atoi(num);
    }
  }

  return instructionCount;
}
// function to convert decimal to binary
void decToBinary(uint64_t n) {
  // array to store binary number
  int binaryNum[64];

  // counter for binary array
  int i = 0;
  while (n > 0) {

    // storing remainder in binary array
    binaryNum[i] = n % 2;
    n = n / 2;
    i++;
  }

  // printing binary array in reverse order
  for (int j = i - 1; j >= 0; j--) {
    printf("%d", binaryNum[j]);
  }
}

int tryAValue(uint64_t regA, int *goal, int goalIndex) {
  // printf("trying ");
  // printf("trying %lu\n", regA);
  // decToBinary(regA);
  // printf("\n");

  int regB = regA % 8;
  int curOutput = goal[goalIndex]; // 0

  regB = regB ^ 3;

  uint64_t regC = regA >> (regB);
  regB = regB ^ 5;
  // regB = 110;

  int lastBitsOfC = regB ^ curOutput; // 110

  if (regC % 8 != lastBitsOfC) {
    return 0;
  }

  return 1;
}

int tryFindingValues(int *goal, int goalIndex, uint64_t regA) {
  int j;
  int newA = -1;
  for (j = 0; j <= 0b111; j++) {
    printf("looking for %d; index= %d\n", goal[goalIndex], goalIndex);
    if (tryAValue((regA << 3) | j, goal, goalIndex)) {
      printf("found newA: %lu\n", (regA << 3) | j);
      printf("new 3 bits of A = ");
      decToBinary(j);
      printf("\n");
      if (goalIndex == 0) {
        return (regA << 3 | j);
      }
      newA = tryFindingValues(goal, goalIndex - 1, (regA << 3) | j);
      if (newA > -1)
        return newA;
    }
  }
  return -1;
}

int main(int argc, char **argv) {
  FILE *file = fopen(argv[1], "r");

  char currChar;
  uint64_t regA;
  int regB;
  int regC;
  int doneWithRegs = 0;
  int instructions[20];
  int instructionCount;
  while ((currChar = fgetc(file)) != EOF) {
    switch (currChar) {
    case 'A':
      break;
    case 'B':
      break;
    case 'C':
      fgetc(file);
      fgetc(file);
      regC = parseNum(file);
      doneWithRegs = 1;
      break;
    case ' ':
      if (doneWithRegs) {
        instructionCount = parseInstructions(file, instructions);
      }
    default:
      break;
    }
  }
  fclose(file);

  // printf("Register A: %d\n", regA);
  // printf("Register B: %d\n", regB);
  // printf("Register C: %d\n", regC);
  // printf("Program: ");
  //  regB = regA % 8
  //.regB = regB ^ 1
  //  regC = regA / 8 == 0
  //  regA = regA / 8 == 0
  //  regB = regB ^ regC ==
  //  regB = regB ^ 5 ==
  //  regB % 8 == 0
  //  regB is a multiple of 8
  regA = 0;
  // for (int i=instructionCount-1; i>=0; i--) {
  //	int j;
  //	int newA;
  //	printf("looking for %d\n", instructions[i]);
  //	for (j=0b000; j<=0b111; j++) {
  //		if (tryAValue((regA << 3) | j, instructions, i)) {
  //			printf("found newA: %lu\n", (regA << 3) | j);
  //			printf("new 3 bits of A = ");
  //			decToBinary(j);
  //			printf("\n");
  //			newA = j;
  //		}
  //	}
  //	regA = (regA << 3) | newA;
  // }
  //
  regA = tryFindingValues(instructions, instructionCount - 1, 0);

  printf("A = %lu\n", regA);
}
