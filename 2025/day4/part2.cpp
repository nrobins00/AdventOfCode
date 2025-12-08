#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

struct Point {
    int row, col;
};

bool checkBounds(vector<string> &grid, int y, int x) {
    if (y < 0 || y >= grid.size()) return false;
    if (x < 0 || x >= grid[0].size()) return false;
    return true;
}

int countRolls(vector<string> &grid, int y, int x) {
    int count = 0;
    for (int dy= -1; dy<2; dy++) {
        for (int dx=-1; dx<2; dx++) {
            if (dy == 0 && dx == 0) continue;
            int nextY = y+dy;
            int nextX = x+dx;
            if (checkBounds(grid, nextY, nextX) && grid[nextY][nextX] == '@') {
                count++;
            }
        }
    }
    return count;
}

int trimRolls(vector<string> &grid) {
    int removed = 0;
    for (int i=0; i<grid.size(); i++) {
        for (int j=0; j<grid[0].size(); j++) {
            if (grid[i][j] == '@' && countRolls(grid, i, j) < 4) {
                removed++;
                grid[i][j] = '.';
            }
        }
    }
    return removed;
}

int main(int argc, char **argv) {
    ifstream file(argv[1]);
    
    string line;
    vector<string> grid;
    while (getline(file, line)) {
        grid.push_back(line);
    }
    int total_removed = 0;
    int removed = 0;
    while ((removed = trimRolls(grid)) > 0) {
        total_removed += removed;

    }
    cout << total_removed << endl;

    ofstream outputFile("good_grid.txt");
    if (!outputFile.is_open()) {
        // Handle error, e.g., print an error message
        std::cerr << "Error opening file!" << std::endl;
        return 1; // Indicate an error
    }

    for (string line : grid) {
        outputFile << line << endl;
    }
    
    // for (string line : grid) {
    //     cout << line << endl;
    // }
}
