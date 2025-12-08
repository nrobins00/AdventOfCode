#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <set>

using namespace std;

struct Range {
    long min, max;
};

int main(int argc, char **argv) {
    ifstream file(argv[1]);
    
    string line;
    vector<string> grid;
    while (getline(file, line)) {
        grid.push_back(line);
    }

    set<int> indicesToCheck;
    int splitsFound = 0;
    for (int row=0; row<grid.size(); row++) {
        for (int col=0; col<grid[row].size(); col++) {
            char c = grid[row][col];
            if (c == 'S') {
                indicesToCheck.insert(col);
                break;
            }
            if (c == '^' && indicesToCheck.find(col) != indicesToCheck.end()) {
                indicesToCheck.erase(col);
                indicesToCheck.insert(col-1);
                indicesToCheck.insert(col+1);
                splitsFound += 1;
            }
        }
    }
    cout << splitsFound << endl;
} 
