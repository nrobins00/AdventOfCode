#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct Oper {
	string lhs, op, rhs, target;
};

inline bool operator<(const Oper& lhs, const Oper& rhs) {
	return lhs.target < rhs.target;
}

unsigned long long getNumFromWires(map<string, long long> wires, char label) {
	unsigned long long ans = 0;
	for (auto wire : wires) {
		//cout << wire.first << ": " << wire.second << endl;
		if (wire.first.find(label) == 0) {
			int pos = stol(wire.first.substr(1, 2));
			ans |= wire.second << pos;
		}
	}
	return ans;
}

long long doRunEasy(map<string, long long> wires, vector<Oper> ops) {
	for (auto oper : ops) {
		if (oper.op == "XOR") {
			wires[oper.target] = wires[oper.lhs] ^ wires[oper.rhs];
		} else if (oper.op == "AND") {
			wires[oper.target] = wires[oper.lhs] & wires[oper.rhs];
		} else if (oper.op == "OR") {
			wires[oper.target] = wires[oper.lhs] | wires[oper.rhs];
		}
	}
	return getNumFromWires(wires, 'z');
}

long long doRunSet(map<string, long long> wires, set<Oper> ops) {
	while (ops.size() > 0) {
		for (auto it = ops.begin(); it != ops.end(); ) {
			Oper oper = *it;
			if (!(wires.count(oper.lhs) && wires.count(oper.rhs))) {
				++it;
				continue;
			}
			//cout << oper.lhs << " " << oper.op << " " << oper.rhs << " -> " << oper.target << endl;
			
			if (oper.op == "XOR") {
				wires[oper.target] = wires[oper.lhs] ^ wires[oper.rhs];
			} else if (oper.op == "AND") {
				wires[oper.target] = wires[oper.lhs] & wires[oper.rhs];
			} else if (oper.op == "OR") {
				wires[oper.target] = wires[oper.lhs] | wires[oper.rhs];
			}
			it = ops.erase(it);
		}
	}
	return getNumFromWires(wires, 'z');
}

long long doRun(map<string, long long> wires, vector<Oper> ops) {
	while (ops.size() > 0) {
		for (auto it = ops.begin(); it != ops.end(); ) {
			Oper oper = *it;
			if (!(wires.count(oper.lhs) && wires.count(oper.rhs))) {
				++it;
				continue;
			}
			//cout << oper.lhs << " " << oper.op << " " << oper.rhs << " -> " << oper.target << endl;
			
			if (oper.op == "XOR") {
				wires[oper.target] = wires[oper.lhs] ^ wires[oper.rhs];
			} else if (oper.op == "AND") {
				wires[oper.target] = wires[oper.lhs] & wires[oper.rhs];
			} else if (oper.op == "OR") {
				wires[oper.target] = wires[oper.lhs] | wires[oper.rhs];
			}
			it = ops.erase(it);
		}
	}
	return getNumFromWires(wires, 'z');
}

set<string> getAllDeps(map<string, set<string>>& deps, string key, set<string>& depsAlreadyFound) {
	if (deps[key].size() == 0) return set<string>();
	if (depsAlreadyFound.find(key) != depsAlreadyFound.end()) {
		return deps[key];
	}
	set<string> newDeps;
	for (auto dep : deps[key]) {
		newDeps.insert(dep);
		//cout << "inserting " << dep << endl;
		deps[dep] = getAllDeps(deps, dep, depsAlreadyFound);
		for (string str : deps[dep]) {
			newDeps.insert(str);
		}
		depsAlreadyFound.insert(dep);
	}
	deps[key] = newDeps;
	return newDeps;
}

bool testSwap(map<string, long long> values, vector<Oper> ops, map<string, set<string>> deps, long long expected, set<string> swaps) {
	vector<string> zs = { "z05", "z06", "z07", "z08","z15","z16","z17","z20","z21","z22" };
	if (swaps.size() == 8) {

		// make sure all zs have a dep here
		bool quitEarly = false;
		for (auto z : zs) {
			bool found = false;
			for (string sw : swaps ) {
				if (deps[z].find(sw) != deps[z].end()){
					found = true;
					break;
				}
			}
			if (!found) {
				quitEarly = true;
				break;
			}
		}
		if (quitEarly) {
			return false;
		}
		cout << "tried swapping with ";
		for (auto sw : swaps ) {
			cout << sw << ",";
		}
		cout <<endl;
		if (doRun(values, ops) == expected) {
			return true;
		} else {
			return false;
		}
	}
	//for (auto it1 = ops.begin(); it1 != ops.end(); ++it1) {
	//	auto val1 = *it1;
	//	string leftOut = val1.target;
	//	if (swaps.find(leftOut) != swaps.end()) { continue; }

	//	for (auto it2 = next(it1, 1); it2 != ops.end(); ++it2) {
	//		auto val2 = *it2;
	//		string rightOut = val2.target;
	//		if (swaps.find(rightOut) != swaps.end()) { continue; }
	//		if (deps[leftOut].find(rightOut) != deps[leftOut].end()) { continue; }
	//		if (deps[rightOut].find(leftOut) != deps[rightOut].end()) {continue;}
	//		
	//		swaps.insert(leftOut);
	//		swaps.insert(rightOut);
	//		it1.target = rightOut;
	//		it2.target = leftOut;
	//	}
	//}
	for (int i=0; i<ops.size()-1; i++) {
		auto val1 = ops[i];
		string leftOut = val1.target;
		if (swaps.size() == 0) {
			cout << "0 testing with " << leftOut << endl;
		}
		if (swaps.size() == 2) {
			cout << "2 testing with " << leftOut << endl;
		}
		if (swaps.size() == 4) {
			cout << "4 testing with " << leftOut << endl;
		}
		if (swaps.size() == 6) {
			cout << "6 testing with " << leftOut << endl;
		}
		if (swaps.find(leftOut) != swaps.end()) { continue; }
		swaps.insert(leftOut);

		for (int j=i+1; j<ops.size(); j++) {
			auto val2 = ops[j];
			string rightOut = val2.target;
			if (swaps.find(rightOut) != swaps.end()) { continue; }
			if (deps[leftOut].find(rightOut) != deps[leftOut].end()) { 
				//cout << rightOut << " is a dep of " << leftOut << endl;
				continue; 
			}
			if (deps[rightOut].find(leftOut) != deps[rightOut].end()) {
				//cout << leftOut << " is a dep of " << rightOut << endl;
				continue;
			}
			
			swaps.insert(rightOut);
			val1.target = rightOut;
			val2.target = leftOut;
			if (testSwap(values, ops, deps, expected, swaps)) {
				return true;
			}
			//set<string> test = testSwap(values, ops, deps, expected, swaps);
			val1.target = leftOut;
			val2.target = rightOut;
			swaps.erase(rightOut);
		}
		swaps.erase(leftOut);
	}
	return false;
}

int main(int argc, char **argv) {
	ifstream file(argv[1]);
	string s;

	map<string, long long> wires;
	bool parsingGates = false;
	vector<Oper> ops;
	while (getline(file, s)) {
		if (s.length() == 0) {
			parsingGates = true;
			continue;
		}
		if (!parsingGates) {
			string wireKey = s.substr(0, 3);
			string bit = s.substr(5, 1);
			wires[wireKey] = stol(bit);
		} else {
			stringstream ss(s);
			string lhs;
			getline(ss, lhs, ' ');
			string op;
			getline(ss, op, ' ');
			string rhs;
			getline(ss, rhs, ' ');
			string gate;
			string _;
			getline(ss, _, ' ');
			string target; 
			getline(ss, target, ' ');
			Oper oper = {
				lhs, op, rhs, target
			};
			ops.push_back(oper);
		}
	}

	for (auto f : wires) {
		cout << f.first << ": " << f.second << endl;
	}

	unsigned long long expected = getNumFromWires(wires, 'x') + getNumFromWires(wires, 'y');
	cout << "expected = " << expected << endl;

	//.vector<Oper> staticOps(ops.begin(), ops.end());


	unsigned long long actual = doRun(wires, ops);
	//for (long long i=0; i< 3712828500000; i++) {
	//	if (i % 1000 == 0) {
	//		cout << "iteration " << i << endl;
	//	}
	//	doRunEasy(wires, ops);
	//}
	//

	long long diff = actual ^ expected;
	cout << "Actual: " << actual << endl;
	cout << "Different bits: " << diff << endl;
	vector<int> diffBits;
	for (int i=0; i<46; i++) {
		unsigned long long one = 1;
		unsigned long long comp = one << i;
		cout << "comp: " << comp  << "; i: " << i << endl;
		if ((diff & comp) > 0) {
			diffBits.push_back(i);
		}
	}

	set<string> diffKeys;
	for (int i : diffBits) {
		//cout << i << endl;
		string target = "z";
		string stringI = to_string(i);
		if (i < 10) {
			target += "0";
			target += stringI;
		} else {
			target += stringI;
		}
		cout << target << endl;
		diffKeys.insert(target);
	}


	set<string> nextLevel;
	bool done = false;
	set<Oper> opsThatMatter;
	map<string, set<string>> deps;
	while (!done) {
		done = true;
		for (string str : diffKeys) {
			for (Oper oper : ops) {
				if (oper.target != str) { continue; }
				opsThatMatter.insert(oper);
				if (oper.lhs.find("x") != 0 && oper.lhs.find("y") != 0) {
					deps[str].insert(oper.lhs);
					nextLevel.insert(oper.lhs);
				}
				if (oper.rhs.find("x") != 0 && oper.rhs.find("y") != 0) {
					deps[str].insert(oper.rhs);
					nextLevel.insert(oper.rhs);
				}
			}
		}
		int found = 0;
		for (string s : nextLevel) {
			if (diffKeys.find(s) == diffKeys.end()) {
				found++;
				done = false;
				diffKeys.insert(s);
			}
		}
		cout << found << " found at this level " << endl;
	}
	cout << "diff Keys: " << endl;
	for (string str : diffKeys) {
		cout << str << endl;
		wires.erase(str);
	}
	cout << "opers that matter: " << endl;
	for (auto op : opsThatMatter) {
		cout << op.lhs << " " << op.op << " " << op.rhs << " -> " << op.target << endl;
	}

	vector<Oper> vecOps(opsThatMatter.begin(), opsThatMatter.end());

	for (auto key : deps) {
		set<string> depsAlreadyFound;
		getAllDeps(deps, key.first, depsAlreadyFound);
		cout << "done getting deps for " << key.first << endl;
	}

	for (auto key : deps) {
		cout << key.first << ": ";
		for (auto str : key.second) {
			cout << str << ",";
		}
		cout << endl;
	}


	//bool swaps = testSwap(wires, vecOps, deps, expected, set<string>());
	//cout << "swaps: " << swaps << endl;


	//for (long long i=0; i< 3712828500000; i++) {
	//	if (i % 1000 == 0) {
	//		cout << "iteration " << i << endl;
	//	}
	//	doRunSet(wires, opsThatMatter);
	//}



	// find gates that feed into each of those

	file.close();
}
