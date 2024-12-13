#include <stdio.h>
#include <stdlib.h>

struct Point {
	int row;
	int col;
};

struct Region {
	char id;
	struct Point *points;
	int capacity;
	int area;
	int perim;
};

struct Grid {
	char *arr;
	int rowCount;
	int colCount;
};

void printGrid(struct Grid grid) {
	for (int i=0; i<grid.rowCount; i++) {
		for (int j=0; j<grid.colCount; j++) {
			printf("%c", grid.arr[i*grid.colCount+j]);
		}
		printf("\n");
	}
}

struct Grid parseGrid(FILE *file) {
	char currChar;
	int rowCount = 0;
	int colCount = 0;
	while ((currChar = fgetc(file)) != EOF) {
		switch (currChar) {
		case '\n':
			rowCount++;
			break;
		default:
			if (rowCount==0) {
				colCount++;
			}
			break;
		}
	}
	rewind(file);
	char *arr = malloc(rowCount*colCount*sizeof(char));
	int row = 0;
	int col = 0;
	while ((currChar = fgetc(file)) != EOF) {
		switch (currChar) {
		case '\n':
			row++;
			col=0;
			break;
		default:
			arr[(row*colCount)+(col)] = currChar;
			col++;
			break;
		}
	}
	struct Grid ret;
	ret.arr = arr;
	ret.colCount = colCount;
	ret.rowCount = rowCount;
	return ret;
}

int checkBounds(struct Grid grid, struct Point point) {
	if (point.row < 0 || point.row >= grid.rowCount) {
		return 0;
	}
	if (point.col < 0 || point.col >= grid.colCount) {
		return 0;
	}
	return 1;
}

char getCharAtPoint(struct Grid grid, struct Point point) {
	if (!checkBounds(grid, point)) {
		return 0;
	}
	return grid.arr[point.row*grid.colCount + point.col];
}

void expandArr(char *arr, int currSize) {
	int newSize = currSize*2;

}

void addPoint(struct Region *region, struct Point point) {
	if (region->area >= region->capacity) {
		region->capacity = region->capacity * 2;
		region->points = (struct Point *)realloc(region->points, 
					   region->capacity*sizeof(struct Point));
	}
	region->points[(region->area)++] = point;
}

int regionContains(struct Region region, struct Point point) { 
	for (int i=0; i<region.area; i++) {
		if (region.points[i].row == point.row &&
			region.points[i].col == point.col) {
			return 1;
		}
	}
	return 0;
}

void continueRegion(struct Grid grid, struct Region *region, struct Point point) {
	if (regionContains(*region, point)) {
		return;
	}
	addPoint(region, point);

	struct Point above = { point.row-1, point.col };
	if (getCharAtPoint(grid, above) == region->id) {
		continueRegion(grid, region, above);
	}

	struct Point right = { point.row, point.col+1 };
	if (getCharAtPoint(grid, right) == region->id) {
		continueRegion(grid, region, right);
	}

	struct Point below = { point.row+1, point.col };
	if (getCharAtPoint(grid, below) == region->id) {
		continueRegion(grid, region, below);
	}

	struct Point left = { point.row, point.col-1 };
	if (getCharAtPoint(grid, left) == region->id) {
		continueRegion(grid, region, left);
	}
}

struct Region processRegion(struct Grid grid, struct Point startPoint) {
	struct Region region;
	region.id = getCharAtPoint(grid, startPoint);
	region.capacity = 10;
	region.area = 0;
	region.points = malloc(region.capacity * sizeof(struct Point));
	
	continueRegion(grid, &region, startPoint);
	return region;
}

void incrementStart(struct Grid grid, struct Point *start) {
	if (start->col == grid.colCount-1) {
		start->col = 0;
		start->row++;
		if (start->row == grid.rowCount) {
			start->col=-1;
			start->row=-1;
		}
		return;
	}
	start->col++;
}

int alreadyProcessedPoint(struct Region *regions, int regionCount, struct Point point) {
	for (int i=0; i<regionCount; i++) {
		if (regionContains(regions[i], point)) {
			return 1;
		}
	}
	return 0;
}

void printPoint(struct Point p){
	printf("(%d, %d)\n", p.row, p.col);
}


int calcSides(struct Region region) {
	int sides = 0;
	for (int i=0; i<region.area; i++) {
		struct Point curPoint = region.points[i];
		int ul=0;
		int u=0;
		int ur=0;
		int r=0;
		int dr=0;
		int d=0;
		int dl=0;
		int l=0;

		struct Point up = {curPoint.row-1, curPoint.col};
		if (regionContains(region, up)) {
			u=1;
		}
		struct Point upRight = {curPoint.row-1, curPoint.col+1};
		if (regionContains(region, upRight)) {
			ur=1;
		}
		struct Point upLeft = {curPoint.row-1, curPoint.col-1};
		if (regionContains(region, upLeft)) {
			ul=1;
		}
		struct Point right = {curPoint.row, curPoint.col+1};
		if (regionContains(region, right)) {
			r=1;
		}
		struct Point downRight = {curPoint.row+1, curPoint.col+1};
		if (regionContains(region, downRight)) {
			dr=1;
		}
		struct Point down = {curPoint.row+1, curPoint.col};
		if (regionContains(region, down)) {
			d=1;
		}
		struct Point downLeft = {curPoint.row+1, curPoint.col-1};
		if (regionContains(region, downLeft)) {
			dl=1;
		}
		struct Point left = {curPoint.row, curPoint.col-1};
		if (regionContains(region, left)) {
			l=1;
		}

		//outer
		//top left
		if (!u && !l) {
			sides++;
			printPoint(curPoint);
		}
		//top right
		if (!u && !r) {
			sides++;
			printPoint(curPoint);
		}	
		//bottom left
		if (!d && !l) {
			sides++;
			printPoint(curPoint);
		}
		//bottom right
		if (!d && !r) {
			sides++;
			printPoint(curPoint);
		}
		//top left
		if (d && r && !dr) {
			sides++;
			printPoint(curPoint);
		}
		//top right
		if (d && l && !dl) {
			sides++;
			printPoint(curPoint);
		}
		//bottom left
		if (u && r && ! ur) {
			sides++;
			printPoint(curPoint);
		}
		//bottom right
		if (u && l && !ul) {
			sides++;
			printPoint(curPoint);
		}
	}
	return sides;
}

int main(int argc, char **argv) {
	FILE *file = fopen(argv[1], "r");

	struct Grid grid = parseGrid(file);

	int regionCount = 0;
	int regionCap = 10;
	struct Region *regions = malloc(regionCap*sizeof(struct Region));
	struct Point start = {0, 0};
	while (checkBounds(grid, start)) {
		struct Region region = processRegion(grid, start);
		if (regionCount == regionCap) {
			regionCap = regionCap*2;
			regions = (struct Region *)realloc(regions, regionCap*sizeof(struct Region));
		}
		regions[regionCount++] = region;
		incrementStart(grid, &start);
		while (start.col >= 0 && alreadyProcessedPoint(regions, regionCount, start)) {
			incrementStart(grid, &start);
		}
		
	}

	int totalPrice = 0;

	for (int i= 0; i<regionCount; i++) {
		struct Region region = regions[i];
		int perim = calcSides(region);
		printf("region %c area: %d; sides: %d\n", region.id, region.area, perim);
		int price = region.area * perim;
		totalPrice += price;
		free(regions[i].points);
	}

	printf("total: %d\n", totalPrice);


	free(grid.arr);
	fclose(file);
}
