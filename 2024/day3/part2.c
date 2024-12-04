#include <stdio.h>
#include <stdlib.h>


int parseSingleChar(FILE *file, char charToParse) {
	char currChar = fgetc(file);
	if (currChar == charToParse) { return 1; }
	ungetc(currChar, file);
	return 0;
}

int isNum(char c) {
	return c >= 48 && c <= 57;
}

int parseNum(FILE *file) {
	char currChar;
	char currString[4];
	int currStringIndex = 0;
	while ((currChar = fgetc(file)) != EOF) {
		if (isNum(currChar)) {
			currString[currStringIndex++] = currChar;
			if (currStringIndex == 4) {
				currString[currStringIndex] = '\0';
				return atoi(currString);
			}
		} else if (currStringIndex > 0) {
			ungetc(currChar, file);
			currString[currStringIndex] = '\0';
			return atoi(currString);
		} else {
			ungetc(currChar, file);
			return 0;
		}
	}

	// ungetc(currChar, file); calling ungetc with EOF does nothing
	return 0;
}

int parseString(FILE *file, char *str) {
	char currChar;
	char *strPtr = str;
	while ((currChar = fgetc(file)) != EOF) {
		if (currChar != strPtr[0]) {
			ungetc(currChar, file);
			return 0;
		}
		strPtr++;
		if (strPtr[0] == '\0') { return 1; }
	}
	// ungetc(currChar, file); calling ungetc with EOF does nothing
	return 0;
}

int main(int argc, char** argv) {
	if (argc != 2) { 
		printf("incorrect arg count\n");
		exit(0);
	}
	FILE *inputFile = fopen(argv[1], "r");

	
	char currChar;
	int lhs;
	int rhs;
	int sum = 0;
	int currProd;
	int enabled=1;
	while ((currChar = fgetc(inputFile)) != EOF) {
		printf("%c", currChar);
		switch (currChar) {
		case 'm':
			ungetc(currChar, inputFile);
			if (!parseString(inputFile, "mul")) { break; }
			if (!parseSingleChar(inputFile, '(')) {break;}
			lhs = parseNum(inputFile);
			if (!lhs) { break; }
			if (!parseSingleChar(inputFile, ',')) { break; }
			rhs = parseNum(inputFile);
			if (!rhs) {break;}
			if (!parseSingleChar(inputFile, ')')) {break;}
			printf("left: %d; right: %d\n", lhs, rhs);
			currProd = lhs * rhs;
			if (enabled) {
				sum += currProd;
			}
			break;
		case 'd':
			ungetc(currChar, inputFile);
			if (!parseString(inputFile, "do")) { break; } 
			if (parseSingleChar(inputFile, '(') && 
					parseSingleChar(inputFile, ')')) {
				enabled = 1; 
				printf("enabled\n");
			} else if (parseString(inputFile, "n't")) {
				if (parseSingleChar(inputFile, '(') &&
					parseSingleChar(inputFile, ')')) {
					enabled = 0;
					printf("disabled\n");
				}
			}
			break;
		default:
			break;
		}
	}

	printf("sum: %d\n", sum);

	fclose(inputFile);
}
