#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

bool isInvalid(long x) {
    string s = to_string(x);
    for (int i=2; i<=s.size(); i++) {
        if (s.size() % i != 0) { continue; }
        string toCmp = s.substr(0,s.size()/i);
        bool matches = true;
        for (int j=1; j<i; j++) {
            string thisOne = s.substr((s.size()/i)*j, s.size()/i);
            if (toCmp != thisOne) { 
                matches = false;
                break; 
            }
        }
        if (matches) {
            return true;
        }
    }
    return false;
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