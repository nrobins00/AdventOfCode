#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StringList {
	int cap, size;
	char **arr;
};

void expandStringList(struct StringList *list) {
	list->cap = list->cap*2;
	list->arr = reallocarray(list->arr, list->cap, sizeof(char*));
}

void addString(struct StringList *list, char *str) {
	if (list->size == list->cap) {
		expandStringList(list);
	}

	list->arr[list->size++] = str;
}


int main(int argc, char **argv) {
FILE *file = fopen(argv[1], "r");

	struct StringList towels;
	towels.cap = 10;
	towels.size = 0;

	towels.arr = malloc(towels.cap*sizeof(char*));
	char currChar;
	char currString[15];
	int currIndex=0;
	int done=0;
	while ((currChar = fgetc(file)) != EOF && !done) {
		switch (currChar) {
		case '\n':
			done=1;
		case ',':
			currString[currIndex] = '\0';
			char *newTowel = malloc((currIndex+1)*sizeof(char));
			strcpy(newTowel, currString);
			addString(&towels, newTowel);
			currIndex = 0;
			break;
		case ' ':
			break;
		default:
			currString[currIndex++] = currChar;
			break;
		}
	}

	for (int i=0; i<towels.size; i++) {
		printf("%s\n", towels.arr[i]);
	}
	fgetc(file); //skip 1 '\n'
	
	struct StringList designs;
	designs.cap = 10;
	designs.size = 0;
	designs.arr = malloc(designs.cap * sizeof(char*));
	currIndex=0;
	while ((currChar = fgetc(file)) != EOF) {
		switch (currChar) {
		case '\n':
			currString[currIndex] = '\0';
			char *newDesign = malloc((currIndex+1)*sizeof(char));
			strcpy(newDesign, currString);
			addString(&designs, newDesign);
			currIndex = 0;
			break;
		default:
			currString[currIndex++] = currChar;
			break;
		}
	}

	for (int i=0; i<designs.size; i++) {
		printf("%s\n", designs.arr[i]);
	}


	fclose(file);
}
