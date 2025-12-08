#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <map>

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

    map<int, long> tlCounts;
    for (int row=0; row<grid.size(); row++) {
        for (int col=0; col<grid[row].size(); col++) {
            char c = grid[row][col];
            if (c == 'S') {
                tlCounts[col] = 1;
                break;
            }
            if (c == '^' && tlCounts.find(col) != tlCounts.end()) {
                long tlCount = tlCounts[col];
                if (tlCounts.find(col-1) != tlCounts.end()) {
                    tlCounts[col-1] += tlCount;
                } else {
                    tlCounts[col-1] = tlCount;
                }
                if (tlCounts.find(col+1) != tlCounts.end()) {
                    tlCounts[col+1] += tlCount;
                } else {
                    tlCounts[col+1] = tlCount;
                }
                tlCounts.erase(col);
            }
        }
    }
    long total = 0;
    for (map<int, long>::iterator it=tlCounts.begin(); it != tlCounts.end(); it++) {
        cout << "key: " << it->first << "; val: " << it->second << endl;
        total += it->second;
    }
    cout << total << endl;
} 
