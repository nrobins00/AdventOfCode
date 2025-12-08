#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

int main(int argc, char **argv) {
    ifstream file(argv[1]);
    
    string line;
    long total = 0;
    while (getline(file, line)) {
        char largestLeft = line[0];
        int largestLeftI = 0;
        for (int i=1; i<line.size()-1; i++) {
            if (line[i] > largestLeft) {
                largestLeft = line[i];
                largestLeftI = i;
            }
        }

        char largestRight = line[largestLeftI+1];
        int largestRightI = largestLeftI+1;
        for (int i=largestRightI+1; i<line.size(); i++) {
            if (line[i] > largestRight) {
                largestRight = line[i];
                largestRightI = i;
            }
        }
        string largestStr = string(1, largestLeft);
        largestStr += largestRight;
        total += stoi(largestStr);
        cout << stoi(largestStr) << endl;
    }
    cout << total << endl;
    
}
