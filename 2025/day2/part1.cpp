#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

bool isInvalid(long x) {
    string s = to_string(x);
    if (s.size() % 2 != 0) { return false; }
    string firstHalf = s.substr(0, s.size()/2);
    string secondHalf = s.substr(s.size()/2, s.size()/2);
    return firstHalf == secondHalf;
}

int main(int argc, char **argv) {
    ifstream file(argv[1]);
    
    string line;

    getline(file, line);

    vector<string> ranges;
    string range;
    istringstream iss(line);
    long invalidCt = 0;
    while (getline(iss, range, ',')) {
        istringstream ss(range);
        string begin, end;
        getline(ss, begin, '-');
        getline(ss, end, '-');
        cout << "begin: " << begin << "; end: " << end << endl;
        long left = stol(begin);
        long right = stol(end);

        for (long i=left; i<=right; i++) {
            if (isInvalid(i)) {
                cout << i << " is invalid" << endl;
                invalidCt += i;
            }
        }
    }
    cout << "invalid: " << invalidCt << endl;


}