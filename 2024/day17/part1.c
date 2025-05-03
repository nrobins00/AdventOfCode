#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

struct Instructions {
  int *arr;
  int size;
};

int isNumeric(char c) { return c >= 48 && c <= 57; }

uint64_t parseNum(FILE *file) {
  char currString[30];
  int index = 0;
  int currChar;
  while (isNumeric(currChar = fgetc(file))) {
    currString[index++] = currChar;
  }
  currString[index] = '\0';
  uint64_t num = strtoull(currString, NULL, 10);
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

uint64_t convertComboOperand(int operand, uint64_t regA, int regB, int regC) {
  switch (operand) {
  case 0:
  case 1:
  case 2:
  case 3:
    return operand;
  case 4:
    return regA;
  case 5:
    return regB;
  case 6:
    return regC;
  case 7:
    printf("wtf\n");
    exit(0);
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
      fgetc(file);
      fgetc(file);
      regA = parseNum(file);
      break;
    case 'B':
      fgetc(file);
      fgetc(file);
      regB = parseNum(file);
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
  int i = 0;
  while (i < instructionCount) {
    int opCode = instructions[i];
    uint64_t operand = instructions[i + 1];

    switch (opCode) {
    case 0:
      // adv
      operand = convertComboOperand(operand, regA, regB, regC);
      regA = regA >> operand;
      break;
    case 1:
      // bxl
      regB = regB ^ operand;
      break;
    case 2:
      // bst
      operand = convertComboOperand(operand, regA, regB, regC);
      regB = operand % 8;
      break;
    case 3:
      // jnz
      if (regA == 0) {
        break;
      }
      i = operand;
      continue; // avoid incrementing
      break;
    case 4:
      // bxc
      regB = regB ^ regC;
      break;
    case 5:
      // out
      operand = convertComboOperand(operand, regA, regB, regC);
      operand = operand % 8;
      printf("%lu,", operand);
      break;
    case 6:
      // bdv
      operand = convertComboOperand(operand, regA, regB, regC);
      regB = regA >> operand;
      break;
    case 7:
      // cdv
      operand = convertComboOperand(operand, regA, regB, regC);
      regC = regA >> operand;
      break;
    }
    i += 2;
  }
  printf("\n");
}
