#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Game {
	int Ax, Ay, Bx, By, X, Y;
};

void printGame(struct Game game) {
	printf("A: X+%d, Y+%d\n", game.Ax, game.Ay);
	printf("B: X+%d, Y+%d\n", game.Bx, game.By);
	printf("Prize: X=%d, Y=%d\n", game.X, game.Y);
}

void popNextNum(struct Game *game, int num) {
	printf("setting %d\n", num);
	if (game->Ax == 0) {
		game->Ax = num;
		return;
	}
	if (game->Ay == 0) {
		game->Ay = num;
		return;
	}
	if (game->Bx == 0) {
		game->Bx = num;
		return;
	}
	if (game->By == 0) {
		game->By = num;
		return;
	}
	if (game->X == 0) {
		game->X = num;
		return;
	}
	if (game->Y == 0) {
		game->Y = num;
		return;
	}
}

struct Game parseGame(FILE *file) {
	char currChar;
	int step = 0;
	int done=0;
	int onNum = 0;
	char curString[7];
	int strIdx = 0;
	struct Game game = { 0, 0, 0, 0, 0, 0 };

	while ((currChar = fgetc(file)) != EOF && !done) {
		switch (currChar) {
		case '+':
			onNum=1;
       			break;
		case '=':
			onNum=1;
			break;
		case ',':
		case '\n':
			if (onNum) {
				curString[strIdx] = '\0';
				popNextNum(&game, atoi(curString));
				strIdx=0;
				onNum=0;
			}
		default:
			if (onNum) {
				curString[strIdx++] = currChar;
			}
			break;
		}
		if (game.Y > 0) {
			done = 1;
		}
	}
	return game;
}

int solveGame(struct Game game) {
	int A = 0;
	int B = 0;

	//A = (game.X*game.By/game.Bx-game.Y)/(game.Ax*game.By/game.Bx-game.Ay);
	A = (game.X*game.By - game.Y*game.Bx) /
		(game.Ax*game.By - game.Ay * game.Bx);
	if (A <= 0) {
		printf("can't solve this game (A=%d) \n", A);
		return 0;
	}
	B = (game.Y - game.Ay*A) / game.By;

	if (A > 100 || B > 100) {
		printf("GREATER THAN 100!! %d, %d\n", A, B);
	}

	if (A*game.Ax + B*game.Bx != game.X ||
		A*game.Ay + B*game.By != game.Y) {
		printf("can't solve this game (A=%d, B=%d)\n", A, B);
		return 0;
	}
	printf("A = %d; B = %d\n", A, B);
	return A*3+B;
}

int main(int argc, char **argv) {
	FILE *file = fopen(argv[1], "r");

	struct Game game;
	uint32_t total = 0;
	while ((game = parseGame(file)).Y > 0) {
		printGame(game);
		total += solveGame(game);
	}

	printf("total: %u\n", total);

	fclose(file);
}
