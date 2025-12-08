#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>

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

        Range newR = Range{stol(left), stol(right)};

        for (int i=0; i<goods.size(); i++) {
            Range existingR = goods[i];
            if (newR.min <= existingR.min && newR.max >= existingR.min)  {
                newR.max = max(newR.max, existingR.max);
                goods.erase(goods.begin() + i);
                i--;
            } else if (newR.min > existingR.min && newR.min <= existingR.max) {
                newR.min = existingR.min;
                newR.max = max(newR.max, existingR.max);
                goods.erase(goods.begin() + i);
                i--;
            }
        }
        goods.push_back(newR);


        //goods.push_back(Range{stol(left), stol(right)});
    }

    long total = 0;
    for (Range r : goods) {
        cout << r.min << " - " << r.max << endl;
        total += r.max - r.min + 1;
    }
    cout << total <<endl;
}
