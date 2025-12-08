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

int countRolls(vector<string> &grid, int y, int x, vector<Point> &adj) {
    int count = 0;
    for (int dy= -1; dy<2; dy++) {
        for (int dx=-1; dx<2; dx++) {
            if (dy == 0 && dx == 0) continue;
            int nextY = y+dy;
            int nextX = x+dx;
            if (checkBounds(grid, nextY, nextX) && grid[nextY][nextX] == '@') {
                count++;
                adj.push_back(Point{nextY, nextX});
            }
        }
    }
    return count;
}

bool visit(vector<string> &grid, vector<vector<int>> &visited, int y, int x) {
    //if (memo[y][x] == 2) { return false; } // pending
  //  cout << y << " " << x << endl;
    // check all adj squares
    vector<Point> adj;
    int adjCount = countRolls(grid, y, x, adj);
    if (adjCount < 4) {
        //cout << "found false counted" << endl;
        visited[y][x] = 0;
        return false;
    }
    visited[y][x] = 1;
    for (Point p : adj) {
        if (visited[p.row][p.col] < 0 && !visit(grid, visited, p.row, p.col) || visited[p.row][p.col] == 0) {
         //   cout << "minus one" << endl;
            adjCount--;
        }
    }
    if (adjCount < 4) {
        visited[y][x] = 0;
    } else {
        visited[y][x] = 1;
    }
    return adjCount >= 4;
}

int main(int argc, char **argv) {
    ifstream file(argv[1]);
    
    string line;
    vector<string> grid;
    while (getline(file, line)) {
        grid.push_back(line);
    }
    int not_removed = 0;
    int total = 0;
    for (int i=0; i<grid.size(); i++) {
        for (int j=0; j<grid[0].size(); j++) {

            vector<vector<int>> visited(grid.size(), vector<int>(grid[0].size(), -1));
            if (grid[i][j] == '@') {
                total++;
                if (visit(grid, visited, i, j)) {
                    not_removed++;
                //    cout << "y: " << i << "; x: " << j << endl;
                } else {
                    grid[i][j] = '.';
                }
            }
        }
    }
    cout << total << endl;
    cout << total - not_removed << endl;

    ofstream outputFile("bad_grid.txt");
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
