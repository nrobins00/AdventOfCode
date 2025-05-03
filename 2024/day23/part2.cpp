#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
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

bool isValid(map<string, vector<string>> conns, vector<string> lan, string addition) {
	for (auto str: lan) {
		if (find(conns[str].begin(), conns[str].end(), addition) == conns[str].end()) {
			return false;
		}
	}
	return true;
}

void printLan(vector<string> lan) {
	for (auto comp : lan) {
		cout << comp << ",";
	}
	//cout << endl;
}

int areClose(vector<string> lan1, vector<string> lan2) {
	int missedIndex = -1;
	for (int i=0; i<lan1.size(); i++) {
		if (lan1[i] != lan2[i]) {
			if (missedIndex > -1) {
				return -1;
			}
			missedIndex = i;
		}
	}
	return missedIndex;
}

void appendInOrder(vector<string>& lan, string str) {
	for (auto it = lan.begin(); it != lan.end(); ++it) {
		if (str < *it) {
			lan.insert(it, str);
			return;
		}
	}
	lan.insert(lan.end(), str);
}


vector<string> findBiggestLan(map<string, vector<string>>& conns, vector<string>& curLan, map<string, vector<string>>::iterator it) {
	vector<string> possibileComps = conns[curLan[0]];

	for (; it!=conns.end(); ++it) {
		string nextComp = (*it).first;
		//cout << "starting at " << nextComp << endl;
		bool valid = true;
		
		if (isValid(conns, curLan, nextComp)) {
			vector<string> nextLan(curLan.begin(), curLan.end());
			nextLan.push_back(nextComp);
			vector<string> theNextNextLan = findBiggestLan(conns, nextLan, it);
			if (theNextNextLan.size() > biggestLanFound.size()) {
				biggestLanFound = theNextNextLan;
			}
		}
	}
	return biggestLanFound;
}

vector<string> findBiggestLanOuter(map<string, vector<string>> conns, vector<vector<string>> threeLans) {
	vector<string> biggestLanFound;
	for (auto lan : threeLans) {
		string lastComp = lan[lan.size()-1];
		auto it = conns.find(lastComp);
		vector<string> next = findBiggestLan(conns, lan, it);
		cout << "done processing a lan " << endl;
		if (next.size() > biggestLanFound.size()) {
			biggestLanFound = next;
			cout << "found one of size " << biggestLanFound.size() << endl;
		}
	}
	return biggestLanFound;
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
	vector<vector<string>> curLans;
	for (auto lan: lans) {
		vector<string> thisLan = { get<0>(lan), get<1>(lan), get<2>(lan) };
		curLans.push_back(thisLan);
	}

	vector<string> sol = findBiggestLanOuter(conns, curLans);
	
	//sort(curLans[0].begin(), curLans[0].end());
	for (auto comp : sol) {
		cout << comp << ",";
	}
	cout << endl;
	cout << "found one of size :" << sol.size() << endl;
}
