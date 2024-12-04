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

	ungetc(currChar, file);
	return 0;
}

int parseMul(FILE *file) {
	char currChar;
	char mul[] = "mul";
	char *mulPtr = mul;
	while ((currChar = fgetc(file)) != EOF) {
		if (currChar != mulPtr[0]) {
			ungetc(currChar, file);
			return 0;
		}
		mulPtr++;
		if (mulPtr[0] == '\0') { return 1; }
	}
	ungetc(currChar, file);
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
	while ((currChar = fgetc(inputFile)) != EOF) {
		printf("%c", currChar);
		switch (currChar) {
		case 'm':
			ungetc(currChar, inputFile);
			if (!parseMul(inputFile)) { break; }
			if (!parseSingleChar(inputFile, '(')) {break;}
			lhs = parseNum(inputFile);
			if (!lhs) { break; }
			if (!parseSingleChar(inputFile, ',')) { break; }
			rhs = parseNum(inputFile);
			if (!rhs) {break;}
			if (!parseSingleChar(inputFile, ')')) {break;}
			printf("left: %d; right: %d\n", lhs, rhs);
			currProd = lhs * rhs;
			sum += currProd;
			break;
		default:
			break;
		}
	}

	printf("sum: %d\n", sum);

	fclose(inputFile);
}
