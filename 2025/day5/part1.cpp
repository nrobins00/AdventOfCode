#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

struct Range {
    long min, max;
};

int main(int argc, char **argv) {
    ifstream file(argv[1]);
    
    string line;
    vector<Range> goods;
    while (getline(file, line)) {
        if (line == "") { cout << "done " << endl; break; }
        cout << line << endl;
        istringstream iss(line);
        int dashI = line.find("-");
        string left = line.substr(0, dashI);
        string right = line.substr(dashI+1, line.size() - dashI);
        goods.push_back(Range{stol(left), stol(right)});
        cout << left << " " << right << endl;
    }

    int freshCount = 0;
    while (getline(file, line)) {
        long toc = stol(line);
        for (Range r : goods) {
            if (toc >= r.min && toc <= r.max) {
                cout << toc << " is fresh" << endl;
                freshCount++;
                break;
            }
        }
    }
    cout << freshCount <<endl;
}
