#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>

using namespace std;

struct Range {
    long min, max;
};

long long calcGroup(const vector<string> &grid, int startI, int endI) {
    bool add = grid[grid.size()-1][startI] == '+';
    long long retVal = add ? 0 : 1;
    for (int col=endI; col>=startI; col--) {
        int power = 0;
        long long num = 0;
        for (int row=grid.size()-2; row>=0; row--) {
            char c = grid[row][col];
            if (c == ' ') {
                continue;
            }
            long digit = c - '0';
            for (int i=0; i<power; i++) {
                digit *= 10;
            }
            num += digit;
            power++;
        }
        cout << "next num: " << num << endl;
        if (add) {
            retVal += num;
        } else {
            retVal *= num;
        }
    }
    return retVal;
}

int main(int argc, char **argv) {
    ifstream file(argv[1]);
    
    string line;
    vector<string> grid;
    while (getline(file, line)) {
        grid.push_back(line);
    }
    int startPtr = 0;
    int endPtr;
    long long final = 0;
    for (int i=1; i<grid[0].size(); i++) {
        char c = grid[grid.size()-1][i];
        if (c == '+' || c == '*') {
            endPtr = i-2;
            final += calcGroup(grid, startPtr, endPtr);
            startPtr = i;
        }
        else if (i == grid[0].size() - 1) {
            endPtr = i;
            final += calcGroup(grid, startPtr, endPtr);
        }
    }

    cout << final << endl;
}
