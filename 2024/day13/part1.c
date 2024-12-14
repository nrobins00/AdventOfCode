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
	int done;
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

int main(int argc, char **argv) {
	FILE *file = fopen(argv[1], "r");

	struct Game game = parseGame(file);
	printGame(game);

	fclose(file);
}
