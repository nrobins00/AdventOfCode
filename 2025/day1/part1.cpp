#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

int main(int argc, char **argv) {
    ifstream file(argv[1]);
    
    string line;

    int cur = 50;
    int zCount = 0;
    while (getline(file, line)) {
        bool left = line[0] == 'L';
        int count = stoi(line.substr(1, line.length()-1));
        int temp = cur;
        if (left) {
            cur -= count;
        } else {
            cur += count;
        }
        cur = cur % 100;
        if (cur == 0) {
            zCount++;
        }
    }

    cout << "zcount: " << zCount << endl;
}