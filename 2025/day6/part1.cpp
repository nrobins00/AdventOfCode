#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>

using namespace std;

struct Range {
    long min, max;
};

int main(int argc, char **argv) {
    ifstream file(argv[1]);
    
    string line;
    vector<vector<string>> grid;
    getline(file, line);
    istringstream iss(line);
    string num;
    vector<vector<long>> lists;
    while (getline(iss, num, ' ')) {
        if (num.empty()) {
            continue;
        }
        lists.push_back(vector<long>({stol(num)}));
    }
    cout << "found " << lists.size() << " lists" <<endl;
    while (getline(file, line)) {
        if (line[0] == '*' || line[0] == '+') {
            break;
        }
        istringstream liss (line);
        for (int i=0; i<lists.size(); i++) {
            long next;
            liss >> next;
            lists[i].push_back(next);
        }
    }
    for (int i=0; i<lists.size(); i++) {
        for (int j=0; j<lists[i].size(); j++) {
            cout << lists[i][j] << " ";
        }
        cout << endl;
    }
    cout << line << endl;
    istringstream operIss(line);
    long total = 0;
    string oper;
    int listI = 0;

    while (getline(operIss, oper, ' ')) { 
        if (oper.empty()) {
            continue;
        }
        long long sum;

        if (oper == "+") {
            sum = accumulate(lists[listI].begin(), lists[listI].end(), 0);
        } else if (oper == "*") {
            sum = lists[listI][0];
            for (int i=1; i<lists[listI].size(); i++) {
                sum *= lists[listI][i];
            }
            //sum = accumulate(lists[listI].begin(), lists[listI].end(), 1, multiplies<long long>());
        } else {
            cout << "encountered an invalid operator " << oper << endl;
            continue;
        }
        cout << "list " << listI << " = " << sum << " " << oper << endl;
        total += sum;
        listI++;
        if (listI == lists.size()) break;
    }
    cout << total <<endl;
}
