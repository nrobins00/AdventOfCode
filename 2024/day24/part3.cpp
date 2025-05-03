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
	file.close();

	for (auto f : wires) {
		cout << f.first << ": " << f.second << endl;
	}

	unsigned long long expected = getNumFromWires(wires, 'x') + getNumFromWires(wires, 'y');
	cout << "expected = " << expected << endl;

	string lastCarry = "wsg";
	for (int i=1; i<45; i++) {
		bool found = false;
		char num[3];
		sprintf(num, "%02d", i);
		string xn = "x"; 
		xn += num;
		string yn = "y";
		yn += num;
		string zn = "z";
		zn += num;
		string thisXor;
		string baseCarry;
		string secondCarry;
		for (auto op : ops) {
			if (op.op == "XOR") {
				if (op.lhs == xn && op.rhs == yn) {
					thisXor = op.target;
				} else if (op.lhs == yn && op.rhs == xn) {
					thisXor = op.target;
				} else {
				}
			} else if (op.op == "AND") {
				if (op.lhs == xn && op.rhs == yn) {
					baseCarry = op.target;
				} else if (op.lhs == yn && op.rhs == xn) {
					baseCarry = op.target;
				} else {
					continue;
				}
			}
		}

		cout << "lastCarry: " << lastCarry << "; xorTarg: " << thisXor << endl;
		bool foundZ;
		string toSwap;
		for (auto op : ops) {

			if (op.op == "XOR") {
				if (op.lhs == lastCarry && op.rhs == thisXor) {
					foundZ = op.target.find("z") == 0;
					// need to swap z with this one
					if (!foundZ) {
						toSwap = op.target;
						op.target = zn;
					}
				}
				if (op.lhs == thisXor && op.rhs == lastCarry) {
					foundZ = op.target.find("z") == 0;
					// need to swap z with this one
					//if (!foundZ) {
					//	toSwap = op.target;
					//	op.target = zn;
					//}
				}
			} else if (op.op == "AND") {
				if (op.lhs == lastCarry && op.rhs == thisXor ||
					op.lhs == thisXor && op.rhs == lastCarry) {
					//if (toSwap.length() > 0) {
					//	op.target = toSwap;
					//}
					secondCarry = op.target;
				}
			}
		}
		cout << "baseCarry : " << baseCarry << endl;
		cout << "secondCarry : " << secondCarry << endl;
		for (auto op: ops) {
			if (op.op == "OR") {
				if (op.lhs == baseCarry && op.rhs == secondCarry ||
					op.lhs == secondCarry & op.rhs == baseCarry) {
					lastCarry = op.target;
					found = true;
				}
			}
		}
		if (!foundZ) {
			cout << "couldn't find Z for " << i << endl;
		}
		if (!found) {
			cout << "couldn't find one for " << i <<endl;
		}
	}



	set<string> nextLevel;
	bool done = false;
	set<Oper> opsThatMatter;
	map<string, set<string>> deps;


	// find gates that feed into each of those

}
