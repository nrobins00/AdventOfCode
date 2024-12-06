#include <stdio.h>
#include <stdlib.h>

void insertAt(int *arr, int val, int at) {
    printf("inserting %d at %d\n", val, at);
    int temp;
    int i;
    for (i=at; arr[i] != -1; i++) {
        //printf("inserting value at %d\n", i);
        temp = arr[i];
        arr[i] = val;
        val = temp;
    }

    temp = arr[i];
    arr[i] = val;
    arr[i+1] = temp;

    printf("sortedArr: ");
    for (int j=0; arr[j] != -1; j++) {

        printf("%d,", arr[j]);
    }
    printf("\n");
}

int comp(int num1, int num2, int **rules) {
    for (int leftRuleIx=0; leftRuleIx<100; leftRuleIx++) {
        int *ruleSet = rules[leftRuleIx];
        for (int rightRuleIx=0; ruleSet[rightRuleIx] != -1; rightRuleIx++) {
            int leftNum = leftRuleIx;
            int rightNum = ruleSet[rightRuleIx];

            if (leftNum == num2 && rightNum == num1) { 
                return 1; 
            }
            if (leftNum == num1 && rightNum == num2) { 
                return -1; 
            }
        }

    }
    printf("couldn't find a rule for %d & %d\n", num1, num2);
    return 0;
}

int sortUpdate(int *update, int **rules) {
    int i=0;
    int sortedArr[30];
    int currNum = update[0];
    sortedArr[0] = currNum; // first value is considered sorted
    sortedArr[1] = -1;
    for (i=1; update[i] != -1; i++) {
        currNum = update[i];
        int inserted = 0;
        int j;
        for (j=0; j<i; j++) {
            printf("comparing %d and %d\n", sortedArr[j], currNum);
            if (comp(sortedArr[j], currNum, rules) > 0) {
                printf("%d sorts after %d\n", sortedArr[j], currNum);
                insertAt(sortedArr, currNum, j);
                inserted = 1;
                break;
            } else {
                printf("%d sorts after %d\n", currNum, sortedArr[j]);
            }
        }
        if (!inserted) {
            insertAt(sortedArr, currNum, i);
        }
    }

    for (int j=0; sortedArr[j] != -1; j++) {
        printf("%d,", sortedArr[j]);
    }
    printf("\n");

    int middleIdx = (i-1)/2;
    return sortedArr[middleIdx];
}

void addRuletoRuleSet(int *ruleSet, int numToAdd) {
    printf("adding rule\n");
    int i=0;
    while (ruleSet[i] != -1) {
        i++;
    }

    ruleSet[i] = numToAdd;
    ruleSet[i+1] = -1;
}

int checkUpdate(int **rules, int *updateToCheck) {
    int i=0;
    int currNum;
    int currRule;
    for (; updateToCheck[i] != -1; i++) {
        currNum = updateToCheck[i];
        int *ruleSet = rules[currNum];
        for (int j=0; ruleSet[j] != -1; j++) {
            int currRule = ruleSet[j];
            // valid rule, so check all nums to the left
            for (int k = 0; k < i; k++) {
                if (updateToCheck[k] == currRule) {
                    return 0;
                }
            }
        }
    }

    int middleIndex = (i-1)/2;
    return updateToCheck[middleIndex];

}

int main(int argc, char **argv) {
    FILE *rulesFile = fopen(argv[1], "r");
    if (rulesFile == NULL) {
        printf("couldn't open rules file\n");
        exit(0);
    }
    FILE *updatesFile = fopen(argv[2], "r");
    if (updatesFile == NULL) {
        printf("couldn't open rules file\n");
        exit(0);
    }

    char currChar;
    char leftNumStr[3];
    int leftNumIndex = 0;
    int rightNumIndex = 0;
    char rightNumStr[3];
    int leftNum = -1;
    int rightNum = -1;
    int **rules = (int**)malloc(100*sizeof(int*));
    for (int i = 0; i < 100; i++) {
        //hold up to 20 rules
        rules[i] = (int*)malloc(100*sizeof(int));
        rules[i][0] = -1;
    }
    while ((currChar = fgetc(rulesFile)) != EOF) {
        switch (currChar) {
        case '\n':
            rightNumStr[rightNumIndex] = '\0';
            rightNum = atoi(rightNumStr);
            leftNumIndex = 0;
            rightNumIndex = 0;

            addRuletoRuleSet(rules[leftNum], rightNum);

            leftNum = -1;
            rightNum = -1;
            break;
        case '|':
            leftNumStr[leftNumIndex] = '\0';
            leftNum = atoi(leftNumStr);
            leftNumIndex = 0;
            break;
        default:
            if (leftNum == -1) {
                    leftNumStr[leftNumIndex++] = currChar;
            } else {
                    rightNumStr[rightNumIndex++] = currChar;
            }
            break;
        }
    }
    fclose(rulesFile);
    printf("got rules\n");


    int lineCount = 0;
    int numCountForLine = 1;
    int maxNumCountForLine = 0;
    while ((currChar = fgetc(updatesFile)) != EOF) {
        if (currChar == '\n') {
            lineCount++;
            if (numCountForLine > maxNumCountForLine) maxNumCountForLine = numCountForLine;
            numCountForLine = 1;
        } else if (currChar == ',') {
            numCountForLine++;
        }
    }

    printf("lineCount: %d\n", lineCount);
    printf("maxLineCount: %d\n", maxNumCountForLine);
    printf("got line count\n");
    int **updatesGrid = (int**)malloc(lineCount*sizeof(int*));
    for (int i = 0; i < lineCount; i++) {
        //TODO: handle malloc fail
        updatesGrid[i] = (int*)malloc(maxNumCountForLine*sizeof(int));
    }

    printf("allocated mem\n");
    char currNumStr[3];
    int currNumIndex=0;
    int currLine = 0;
    int currLineIndex = 0;
    rewind(updatesFile);
    while ((currChar = fgetc(updatesFile)) != EOF) {
        switch (currChar) {
        case '\n':
            currNumStr[currNumIndex] = '\0';
            updatesGrid[currLine][currLineIndex++] = atoi(currNumStr);
            updatesGrid[currLine][currLineIndex] = -1; // sentinel value
            for(int i = 0; i < currLineIndex; i++) {
                    printf("%d, ", updatesGrid[currLine][i]);
            }
                printf("\n");
            currLine++;
            currNumIndex = 0;
            currLineIndex = 0;
            break;
        case ',':
            currNumStr[currNumIndex] = '\0';
            updatesGrid[currLine][currLineIndex++] = atoi(currNumStr);
            currNumIndex = 0;
            break;
        default:
            currNumStr[currNumIndex++] = currChar;
            break;
        }

    }
    fclose(updatesFile);
    printf("got all updates into grid\n");

    int sum = 0;
    for (int i = 0; i < lineCount; i++) {
        if (!checkUpdate(rules, updatesGrid[i])) {
            printf("\n\n");
            printf("sorting this one: ");
            for (int j=0; updatesGrid[i][j] != -1; j++) {
                printf("%d,", updatesGrid[i][j]);
            }
            printf("\n");
            int valToAdd = sortUpdate(updatesGrid[i], rules);
            printf("adding %d from line %d\n", valToAdd, i);
            sum += valToAdd;
        }
    }
    printf("sum: %d\n", sum);

}
