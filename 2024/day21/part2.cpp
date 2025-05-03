#include <cstdint>
#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <fstream>
using namespace std;

char numPad[4][3] = { 
			{ '7', '8', '9' },
			{ '4', '5', '6' },
			{ '1', '2', '3' },
			{ ' ', '0', 'A' }
		    };

char dirPad[2][3] = { 
			{ ' ', '^', 'A' },
			{ '<', 'v', '>' },
		    };

struct String {
	char *arr;
	uint64_t size, cap;
};

int charToNum(char c) {
	switch (c) {
	case '^':
		return 0;
		break;
	case '>':
		return 1;
		break;
	case 'v':
		return 2;
		break;
	case '<':
		return 3;
		break;
	case 'A':
		return 4;
		break;
	}
	return -1;
}

uint64_t checkMemo(uint64_t *memo, char c, int layers) {
	int index = charToNum(c);
	index = index*26 + layers;
	return memo[index];
}

void setMemo(uint64_t *memo, char c, int layers, uint64_t val) {
	int index = charToNum(c);
	index = index*26 + layers;
	memo[index] = val;
}

void printBackOneLevel(string input) {
	int curRow = 0;
	int curCol = 2;
	for (int i=0; i<input.length(); i++) {
		switch (input[i]) {
		case 'A':
			//printf("%c", dirPad[curRow][curCol]);
			break;
		case '^':
			curRow--;
			break;
		case '>':
			curCol++;
			break;
		case 'v':
			curRow++;
			break;
		case '<':
			curCol--;
			break;
		}
	}
	printf("\n");
}

void findNumPadPos(char c, int *row, int *col) {
	int curRow, curCol;
	for (curRow = 0; curRow < 4; curRow++) {
		for (curCol = 0; curCol < 3; curCol++) {
			if (numPad[curRow][curCol] == c) {
				*row = curRow;
				*col = curCol;
				return;
			}
		}
	}
}

void findDirPadPos(char c, int *row, int *col) {
	int curRow, curCol;
	for (curRow = 0; curRow < 2; curRow++) {
		for (curCol = 0; curCol < 3; curCol++) {
			if (dirPad[curRow][curCol] == c) {
				*row = curRow;
				*col = curCol;
				return;
			}
		}
	}
}

string numPadMove(int startRow, int startCol, int endRow, int endCol) {
	//printf("moving (%d, %d) to (%d, %d)\n", startRow, startCol, endRow, endCol);
	string moves;
	int startStartRow = startRow;
	int moveDownFirst = startCol > 0 || endRow < 3;

	if (startStartRow < 3 || endCol > 0) {
		//move left
		while (startCol > endCol) {
			moves.append("<");
			startCol--;
		}
	}
	if (moveDownFirst) {
		while (endRow > startRow) {
			moves.append("v");
			startRow++;
		}
	}
	//move up
	while (startRow > endRow) {
		moves.append("^");
		startRow--;
	}

	//move right
	while (endCol > startCol) {
		moves.append(">");
		startCol++;
	}

	while (startCol > endCol) {
		moves.append("<");
		startCol--;
	}

	
	// move down
	while (endRow > startRow) {
		moves.append("v");
		startRow++;
	}

	return moves;
}

string dirPadMove(int startRow, int startCol, int endRow, int endCol) {
	string moves;

	int moveLeftFirst = startRow > 0 || (endCol >= 1);
	int moveUpFirst = startCol > 0 || endRow == 1;

	if (moveLeftFirst) {
		while (startCol > endCol) {
			moves.append("<");
			startCol--;
		}
	}

	if (moveUpFirst) {
		while (startRow > endRow) {
			moves.append("^");
			startRow--;
		}
	}
	// move down
	while (endRow > startRow) {
		moves.append("v");
		startRow++;
	}

	//move right
	while (endCol > startCol) {
		moves.append(">");
		startCol++;
	}

	//move left
	while (startCol > endCol) {
		moves.append("<");
		startCol--;
	}
	
	//move up
	while (startRow > endRow) {
		moves.append("^");
		startRow--;
	}
	return moves;
}

string findNumPadInput(string goal) {
	int curRow = 3;
	int curCol = 2;
	string allMoves;
	for (int i=0; i<goal.length(); i++) {
		int nextRow;
		int nextCol;
		findNumPadPos(goal[i], &nextRow, &nextCol);
		string moves = numPadMove(curRow, curCol, nextRow, nextCol);
		moves.append("A");
		allMoves.append(moves);
		curRow = nextRow;
		curCol = nextCol;
	}
	//printf("%s\n", allMoves.arr);
	return allMoves;
}

string findDirPadInput(string goal, char startC) {
	int curRow;
	int curCol;
	findDirPadPos(startC, &curRow, &curCol);

	string allMoves;
	for (int i=0; i<goal.length(); i++) {
		int nextRow;
		int nextCol;
		findDirPadPos(goal[i], &nextRow, &nextCol);
		string moves = dirPadMove(curRow, curCol, nextRow, nextCol);
		moves.append("A");
		allMoves.append(moves);
		curRow = nextRow;
		curCol = nextCol;
	}
	return allMoves;
}

long long countInputsNeeded(char c, vector<char>& curPos, int numLayers, map<pair<char, int>, long long>& memo) {
	if (curPos.at(numLayers) == 'A') {
		//printf("using cached value!\n");
		long long memVal = memo[pair(c, numLayers)];
		if (memVal > 0) {
			(*curPos)[numLayers] = c;
			return memVal;
		}
	}
	//printf("countInputsNeeded: to %c, from %c, %d\n", c, curPos[numLayers], numLayers);
	if (numLayers == 0) {
		if (curPos->at(numLayers) == 'A') {
			(*memo)[pair(c, numLayers)] = 1;
			//memo->insert({pair(c, numLayers), 1});
		}
		return 1;
	}
	string strToFind = "c";

	string nextLevel = findDirPadInput(strToFind, curPos->at(numLayers));
	//printf("now checking %s at %d layers\n", nextLevel.arr, numLayers-1);
	long long total = 0;
	for (long long i=0; i<nextLevel.length(); i++) {
		total += countInputsNeeded(nextLevel[i], curPos, numLayers-1, memo);
		(*curPos)[numLayers-1] = nextLevel[i];
	}
	if ((*curPos)[numLayers] == 'A') {
		(*memo)[pair(c, numLayers)] = total;
	}
	return total;
}


int main(int argc, char **argv) {
	//FILE *file = fopen(argv[1], "r");
	////char *str = "<A>Av<<AA>^AA>AvAA^A<vAAA>^A";
	////printBackOneLevel(str);
	////fclose(file);
	////exit(0);

	//string codes[5];
	//char currChar;
	//int currCode = 0;
	//int currCodeIdx = 0;
	//while ((currChar = fgetc(file)) != EOF) {
	//	if (currChar == '\n') {
	//		codes[currCode][currCodeIdx] = '\0';
	//		currCode++;
	//		currCodeIdx = 0;
	//	} else {
	//		codes[currCode][currCodeIdx++] = currChar;
	//	}
	//}
	//fclose(file);

	string codes[5];
	int curCode = 0;
	ifstream file(argv[1]);
	string curLine;
	while (getline(file, curLine)) {
		codes[curCode++] = curLine;
	}

	map<pair<char, int>, long long> memo(700, 0);
	vector<char> curPos(26, 'A');

	int numRobotsWithDir = 25;
	uint64_t score = 0;
	for (int i=0; i<5; i++) {
		cout << codes[i] << endl;
		string inputs = findNumPadInput(codes[i]);
		cout << "checking " << inputs << " at " << numRobotsWithDir << " layers" << endl;
		
		uint64_t thisScore = 0;
		for (int i=0; i<inputs.length(); i++) {
			thisScore += countInputsNeeded(inputs[i], &curPos, numRobotsWithDir, memo);
			curPos[numRobotsWithDir] = inputs[i];
		}
		int numCode = strtol(codes[i], NULL, 10);
		printf("num part = %d\n", numCode);
		uint64_t inputLen = thisScore;
		printf("length of input = %lu\n", inputLen);

		score += numCode * inputLen;

		free(inputs.arr);
	}

	printf("score = %lu\n", score);

}
