#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

int findLargestChar(string &input, int digitsLeft) {
    char largest = input[0];
    int largestI = 0;
    for (int i=1; i<input.size()-digitsLeft; i++) {
        if (input[i] > largest) {
            largestI = i;
            largest = input[i];
        }
    }
    return largestI;
}

int main(int argc, char **argv) {
    ifstream file(argv[1]);
    
    string line;
    long total = 0;
    while (getline(file, line)) {

        string largestStr;

        for (int i=11; i>=0; i--) {
            int nextI = findLargestChar(line, i);
            largestStr += line[nextI];
            line.erase(0, nextI+1);
        }
        total += stol(largestStr);
        cout << stol(largestStr) << endl;
    }
    cout << total << endl;
    
}
