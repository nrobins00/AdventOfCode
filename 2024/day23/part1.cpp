#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <unordered_set>
#include <variant>
#include <vector>
using std::ifstream;
using std::map;
using std::string;

using namespace std;

bool areConnected(map<string, vector<string>>& conns, string s1, string s2) {
	for (auto s : conns [s1]) {
		if (s == s2) {
			return true;
		}
	}

	return false;
}

tuple<string, string, string> makeSortedTuple(vector<string> vec) {
	sort(vec.begin(), vec.end());
	return make_tuple(vec[0], vec[1], vec[2]);
}


int main(int argc, char **argv) {
	ifstream file(argv[1]);

	string s;
	map<string, vector<string>> conns;
	while (getline(file, s)) {
		string lhs = s.substr(0, 2);
		string rhs = s.substr(3, 2);
		cout << lhs << " and " << rhs << endl;
		conns[lhs].push_back(rhs);
		conns[rhs].push_back(lhs);
	}
	file.close();

	set<tuple<string, string, string>> lans;
	for (auto f : conns) {
		for (auto p : f.second) {
			vector<string> pConns = conns[p];
			for (auto t : conns[p]) {
				for (auto tP : conns[t]) {
					if (tP == f.first) {
						vector<string> vec = { f.first, p, t };
						tuple<string, string, string> tup = makeSortedTuple(vec);
						if (lans.find(tup) == lans.end()) {
							lans.insert(tup);
						}
					}
				}
			}
		}

	}

	int count;
	for (auto lan : lans) {
		if (get<0>(lan).find("t") == 0) {
			cout << get<0>(lan) << "," << get<1>(lan) << "," << get<2>(lan) << endl;
			count++;
			continue;
		}
		if (get<1>(lan).find("t") == 0) {
			cout << get<0>(lan) << "," << get<1>(lan) << "," << get<2>(lan) << endl;
			count++;
			continue;
		}
		if (get<2>(lan).find("t") == 0) {
			cout << get<0>(lan) << "," << get<1>(lan) << "," << get<2>(lan) << endl;
			count++;
			continue;
		}
	}

	cout << "size: " << lans.size() << endl;
	cout << "the ts: " << count << endl;
}
