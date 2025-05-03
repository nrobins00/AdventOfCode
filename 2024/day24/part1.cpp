#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct Oper {
	string lhs, op, rhs, target;
};

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

	while (ops.size() > 0) {
		for (auto it = ops.begin(); it != ops.end(); ) {
			Oper oper = *it;
			if (!(wires.count(oper.lhs) && wires.count(oper.rhs))) {
				++it;
				continue;
			}
			cout << oper.lhs << " " << oper.op << " " << oper.rhs << " -> " << oper.target << endl;
			
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

	unsigned long long ans = 0;
	for (auto wire : wires) {
		//cout << wire.first << ": " << wire.second << endl;
		if (wire.first.find("z") == 0) {
			int pos = stol(wire.first.substr(1, 2));
			cout << wire.first << ": " << wire.second << " pos: " << pos << endl;
			ans |= wire.second << pos;
		}
	}
	cout << "ANSWER: " << ans << endl;

	file.close();
}
