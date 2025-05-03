#include <stdio.h>
#include <stdlib.h>

struct Grid {
	char *arr;
	int rows, cols;
};

struct Point {
	int row, col;
};

struct PointList {
	struct Point *arr;
	int size, cap;
};

void addPoint(struct PointList *path, struct Point point) {
	if (path->size == path->cap) {
		path->cap *= 2;
		path->arr = reallocarray(path->arr, path->cap, sizeof(struct Point));
	}

	path->arr[path->size++] = point;
}

char getCharAt(struct Grid grid, struct Point point) {
	if (point.row < 0 || point.row > grid.rows-1) { return '#'; }
	if (point.col < 0 || point.col > grid.cols-1) { return '#'; }
      return grid.arr[point.row*grid.cols+point.col];
}

int pointsEqual(struct Point p1, struct Point p2) {
	return p1.row == p2.row && p1.col == p2.col;
}

void findPath(struct Grid grid, struct PointList *path) {
	for (int row=0; row<grid.rows; row++) {
		for (int col=0; col<grid.cols; col++) {
			struct Point cur = { row, col };
			if (getCharAt(grid, cur) == 'S') {
				addPoint(path, cur);
				break;
			}
		}
	}

	int curSpot = -1;
	while (getCharAt(grid, path->arr[path->size-1]) != 'E') {
		curSpot++;
		struct Point curPoint = path->arr[curSpot];
		struct Point lastPoint = { -1, -1 };
		if (curSpot > 0) {
			lastPoint = path->arr[curSpot-1];
		}
		struct Point nextPoint;
		// UP
		nextPoint.row = curPoint.row-1;
		nextPoint.col = curPoint.col;
		if (!pointsEqual(lastPoint, nextPoint) && (getCharAt(grid, nextPoint) != '#')) {
			addPoint(path, nextPoint);
			continue;
		}
		// RIGHT
		nextPoint.row = curPoint.row;
		nextPoint.col = curPoint.col+1;
		if (!pointsEqual(lastPoint, nextPoint) && (getCharAt(grid, nextPoint) != '#')) {
			addPoint(path, nextPoint);
			continue;
		}
		// DOWN
		nextPoint.row = curPoint.row+1;
		nextPoint.col = curPoint.col;
		if (!pointsEqual(lastPoint, nextPoint) && (getCharAt(grid, nextPoint) != '#')) {
			addPoint(path, nextPoint);
			continue;
		}
		// LEFT
		nextPoint.row = curPoint.row;
		nextPoint.col = curPoint.col-1;
		if (!pointsEqual(lastPoint, nextPoint) && (getCharAt(grid, nextPoint) != '#')) {
			addPoint(path, nextPoint);
			continue;
		}
	}
}

int findPathIndex(struct PointList path, struct Point point) {
	for (int i=0; i<path.size; i++) {
		struct Point checkPoint = path.arr[i];
		if (pointsEqual(checkPoint, point)) {
			return i;
		}
	}
	return -1;
}

int absDiff(int a, int b) {
	if (a > b) {
		return a - b;
	}
	return b - a;
}

int pointDiff(struct Point p1, struct Point p2) {
	int dist = 0;
	dist += absDiff(p1.row, p2.row);
	dist += absDiff(p1.col, p2.col);
	return dist;
}

int findShortcuts(struct Grid grid, struct PointList path) {
	int found=0;
	for (int i=0; i<path.size; i++) {
		struct Point start = path.arr[i];
		for (int j=i+1; j<path.size; j++) {
			struct Point end = path.arr[j];
			int shortcutDist = pointDiff(start, end);
			int pathDist = j-i;
			if (shortcutDist > 20) { continue; }
			int saved = pathDist - shortcutDist ;
			if (saved > 99) {
				found++;
			}
		}
	}
	return found;
}

int main(int argc, char **argv) {
	FILE *file = fopen(argv[1], "r");

	char currChar;
	int rows = 0;
	int cols = 0;
	while ((currChar = fgetc(file)) != EOF) {
		if (currChar == '\n') {
			rows++;
		} else if (rows == 0) {
			cols++;
		}
	}
	rewind(file);
	
	struct Grid grid;
	grid.rows = rows;
	grid.cols = cols;
	grid.arr= (char*)malloc(rows * cols * sizeof(char));

	int curRow = 0;
	int curCol = 0;
	while ((currChar = fgetc(file)) != EOF) {
		if (currChar == '\n') {
			curRow++;
			curCol = 0;
		} else {
			grid.arr[curRow*cols+curCol] = currChar;
			curCol++;
		}
	}
	fclose(file);

	for (int row=0; row<grid.rows; row++) {
		for (int col=0; col<grid.cols; col++) {
			printf("%c", grid.arr[row*grid.cols+col]);
		}
		printf("\n");
	}

	struct PointList path;
	path.size = 0;
	path.cap = 10;
	path.arr = (struct Point *)malloc(path.cap*sizeof(struct Point));
	findPath(grid, &path);

	//for (int i=0; i<path.size; i++) {
	//	printf("(%d, %d)\n", path.arr[i].row, path.arr[i].col);
	//}

	// now find shortcuts
	int total = findShortcuts(grid, path);
	printf("found = %d\n", total);

	free(grid.arr);
	free(path.arr);
}
