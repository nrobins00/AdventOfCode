#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StringList {
	int cap, size;
	char **arr;
};

struct IndexList{
	int cap, size;
	int *arr;
};

void expandStringList(struct StringList *list) {
	list->cap = list->cap*2;
	list->arr = (char**)reallocarray(list->arr, list->cap, sizeof(char*));
}

void addString(struct StringList *list, char *str) {
	if (list->size == list->cap) {
		expandStringList(list);
	}

	list->arr[list->size++] = str;
}

int checkDesign(struct StringList towels, char *design, int curIndex, int *checkedSpots) {
	if (curIndex > 200) {
		printf("went over 200!!!\n");
	}
	if (checkedSpots[curIndex]) {
		return 0;
	}
	checkedSpots[curIndex] = 1;
	for (int i=0; i<towels.size; i++) {
		char *towel = towels.arr[i];

		if (strlen(design) < strlen(towel)) {
			continue;
		}

		//printf("comparing %s and %s\n", towel, design);
		if (!strcmp(towel, design)) {
			return 1;
		}

		//printf("comparing %s and %s\n", towel, design);
		int foundMatch = !strncmp(towel, design, strlen(towel));

		if (!foundMatch) continue;
		//printf("found match: %s and %s\n", towel, design);

		if (checkDesign(towels, &design[strlen(towel)], curIndex+strlen(towel), checkedSpots)) {
			return 1;
		}
	}

	return 0;
}


int main(int argc, char **argv) {
	FILE *file = fopen(argv[1], "r");

	struct StringList towels;
	towels.cap = 10;
	towels.size = 0;

	towels.arr = (char**)malloc(towels.cap*sizeof(char*));
	char currChar;
	char currString[200];
	int currIndex=0;
	int done=0;
	while ((currChar = fgetc(file)) != EOF && !done) {
		switch (currChar) {
		case '\n':
			done=1;
		case ',':
			currString[currIndex] = '\0';
			char *newTowel = (char*)malloc((currIndex+1)*sizeof(char));
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
	//fgetc(file); //skip 1 '\n'
	
	struct StringList designs;
	designs.cap = 10;
	designs.size = 0;
	designs.arr = (char **)malloc(designs.cap * sizeof(char*));
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

	int totalValid = 0;
	for (int i=0; i<designs.size; i++) {
		if (strlen(designs.arr[i]) > 200) {
			printf("OHHHH\n");
		}
		int checkedList[200];
		for (int j=0; j<200; j++) {
			checkedList[j] = 0;
		}
		printf("%s\n", designs.arr[i]);
		int valid = checkDesign(towels, designs.arr[i], 0, checkedList);
		totalValid += valid;
		printf("valid = %d\n", valid);
	}
	printf("total = %d\n", totalValid);
	free(designs.arr);
	free(towels.arr);


	fclose(file);
}
