
#include <stdio.h>
int main() {
	int prevX = 1;
	for (int x = 0; x < prevX ; x-- ) {
		prevX = x;
	}
	printf("prevX: %d\n", prevX);
}
