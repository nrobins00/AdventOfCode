#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

int countRolls(vector<string> &grid, int y, int x) {
    int count = 0;
    if (y-1 >= 0 && x-1 >= 0 && grid[y-1][x-1] == '@') {
        count++;
    }
    if (y-1 >=0 && grid[y-1][x] == '@') {
        count++;
    }
    if (x+1 <= grid[0].size() && y-1 >=0 && grid[y-1][x+1] == '@') {
        count++;
    }
    if (x-1 >=0 && grid[y][x-1] == '@') {
        count++;

    }
    if (x+1 <= grid[0].size() && grid[y][x+1] == '@') {
        count++;
    }
    if (x-1 >=0 && y+1 <=grid.size() && grid[y+1][x-1] == '@') {
        count++;
    }
    if (y+1 <= grid.size() && grid[y+1][x] == '@') {
        count++;
    }
    if (y+1 <= grid.size() && x+1 <=grid[0].size() && grid[y+1][x+1] == '@') {
        count++;
    }
    return count;
}

int main(int argc, char **argv) {
    ifstream file(argv[1]);
    
    string line;
    vector<string> grid;
    while (getline(file, line)) {
        grid.push_back(line);
    }
    int total = 0;
    for (int i=0; i<grid.size(); i++) {
        for (int j=0; j<grid[0].size(); j++) {
            if (grid[i][j] == '@' && countRolls(grid, i, j) < 4) {
                total++;
                cout << "y: " << i << "; x: " << j << endl;
            }
        }
    }
    cout << total << endl;
}
