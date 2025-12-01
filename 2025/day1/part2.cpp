#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv) {
    ifstream file(argv[1]);
    
    string line;
    bool left = false;
    int cur = 50;
    int zCount = 0;
    while (getline(file, line)) {
        //cout << line << endl;

        bool left = line[0] == 'L';
        int count = stoi(line.substr(1, line.length()-1));
        cout << "count: " << count << endl;
        int extracrosses = count / 100; // input file has some 100%
        zCount += extracrosses;
        count = count % 100; 
        int temp = cur;
        if (left) {
            cur -= count;
            if (cur < 0) {
                cur = 100 - abs(cur);
            }
            if (cur > temp && temp != 0 || cur == 0) {
                zCount++;
            }
        } else {
            cur += count;
            cur = cur % 100;
            if (cur < temp) {
                zCount++;
            }
        }
        cout << cur << endl;

        
        //cout << "num1: " << leftNum << "; num2: " << rightNum << endl;
    }

    cout << "zcount: " << zCount << endl;
}