#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Equation {
    uint64_t total;
    int operandCount;
    int operands[30];
    int operatorCount;
    char operators[29]; // always one less than operands
};

void printEquation(struct Equation *eq) {
    printf("%lu: ", eq->total);
    for (int i=0; i<eq->operandCount; i++) {
        printf("%d", eq->operands[i]);
        if (i < eq->operatorCount) {
            printf(" %c ", eq->operators[i]);
        }
    }
    printf("\n");
}

int readEquationFromFile(struct Equation *eq, FILE *file) {
    char currChar;
    int done=0;
    char currString[20];
    int currStringIdx = 0;
    eq->operandCount = 0;
    eq->operatorCount = 0;
    while (!done && (currChar = fgetc(file)) != EOF) {
        switch (currChar) {
        case '\n':
            eq->operatorCount = eq->operandCount; // about to add 1 more
            done = 1;
        case ' ':
            currString[currStringIdx] = '\0';
            eq->operands[eq->operandCount++] = atoi(currString);
            currStringIdx=0;
            break;
        case ':':
            currString[currStringIdx] = '\0';
            currStringIdx=0;
            eq->total = strtoull(currString, NULL, 10);
            fgetc(file); // skip a ' '
            break;
        default:
            currString[currStringIdx++] = currChar;
            break;
        }
    }
    return done;
}

int checkEquation(struct Equation *eq) {
    //printEquation(eq);
    uint64_t sum=eq->operands[0];
    for (int i=0; i<eq->operatorCount; i++) {
        char operator = eq->operators[i];
        if (operator == '+') {
            sum += eq->operands[i+1];
        } else if (operator == '*') {
            sum *= eq->operands[i+1];
        }
    }
    return sum == eq->total;
}

int checkPossibilities(struct Equation *eq, int operatorsSet) {
    if (operatorsSet == eq->operatorCount) {
        return checkEquation(eq);
    }

    eq->operators[operatorsSet] = '+';
    if (checkPossibilities(eq, operatorsSet+1)) {
        return 1;
    }

    eq->operators[operatorsSet] = '*';
    return checkPossibilities(eq, operatorsSet+1);
}

int main(int argc, char** argv) {
	if (argc != 2) { 
		printf("incorrect arg count\n");
		exit(0);
	}

	FILE *inputFile = fopen(argv[1], "r");
    struct Equation equation;
    uint64_t sum = 0;
    uint64_t prevSum = sum;
    while (readEquationFromFile(&equation, inputFile)) {
        if (checkPossibilities(&equation, 0)) {
            //printf("adding %lu\n", equation.total);
            sum += equation.total;
            if (sum <= prevSum) {
                printf("wtf\n");
                exit(0);
            }
            prevSum = sum;
        }
    }
    fclose(inputFile);
    printf("sum: %lu\n", sum);

}
