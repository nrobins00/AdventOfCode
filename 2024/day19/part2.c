#include <stdint.h>
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

uint64_t checkDesign(struct StringList towels, char *design, uint64_t *checkedSpots, uint64_t *totalPossible) {
	uint64_t poss = 0;
	if (checkedSpots[strlen(design)] > 0) {
		printf("found cache for %s: %lu\n", design, checkedSpots[strlen(design)]);
		return checkedSpots[strlen(design)];
	}
	//printf("checking %s with length %d\n", design, designLen);
	for (int i=0; i<towels.size; i++) {
		char *towel = towels.arr[i];

		if (strlen(design) < strlen(towel)) {
			continue;
		}

		//printf("comparing %s and %s\n", towel, design);
		char *toCmp = &design[strlen(design)-strlen(towel)];
		if (!strcmp(towel, toCmp) && strlen(design) == strlen(towel)) {
			(*totalPossible)++;
			//printf("found one!\n");
			printf("found match: %s and %s\n", towel, toCmp);
			poss++;
		}

		//printf("comparing %s and %s\n", towel, design);
		int foundMatch = !strncmp(towel, toCmp, strlen(towel));

		if (!foundMatch) continue;
		printf("found match: %s and %s\n", towel, toCmp);

		if (strlen(design) > strlen(towel)) {
			char *newDesign = (char *)malloc((strlen(design)+1-strlen(towel))*sizeof(char));
			strncpy(newDesign, design, strlen(design)-strlen(towel));
			newDesign[strlen(design)-strlen(towel)] = '\0';
			//printf("%s\n", newDesign);
			uint64_t ret = checkDesign(towels, newDesign, checkedSpots, totalPossible);
			printf("%s returned %lu\n", newDesign, ret);
			poss += ret;
			free(newDesign);
		}
	}
	checkedSpots[strlen(design)] = poss;
	return poss;
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

	uint64_t totalValid = 0;
	for (int i=0; i<designs.size; i++) {
		if (strlen(designs.arr[i]) > 200) {
			printf("OHHHH\n");
		}
		uint64_t checkedList[200];
		for (int j=0; j<200; j++) {
			checkedList[j] = 0;
		}
		printf("%s\n", designs.arr[i]);
		uint64_t validCnt = 0;
		uint64_t valid = checkDesign(towels, designs.arr[i], checkedList, &validCnt);
		validCnt = checkedList[strlen(designs.arr[i])];
		totalValid += validCnt;
		printf("valid = %lu\n", validCnt);
	}
	printf("total = %lu\n", totalValid);
	free(designs.arr);
	free(towels.arr);


	fclose(file);
}
