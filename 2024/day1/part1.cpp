#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int main() {
	ifstream f("example.txt");

	if (!f.is_open()) {
		cerr << "Error opening the file!";
		return 1;
	}

	string s;
	size_t strLen = 0;


	while (getline(f, s)) {
		stringstream ss(s);
		string num;
		while (!ss.eof()) {
			getline(ss, num, ' ');
			if (num.length() > 0) {
				cout << num << endl;
			}
		}
	}


	f.close();
	return 0;
}
