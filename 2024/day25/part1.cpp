#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
int main(int argc, char **argv) {
	ifstream file(argv[1]);

	string s;
	vector<int> heights(5);
	vector<vector<int>> keys;
	vector<vector<int>> locks;
	int keyOrLock = 0;
	while (getline(file, s)) {
		cout << s << ": " << s.length() << endl;
		if (s.length() == 0) {
			for (int i=0; i<5; i++) {
				heights[i]--;
			}
			if (keyOrLock == -1) {
				keys.push_back(vector<int>(heights));
			} else {
				locks.push_back(vector<int>(heights));
			}
			keyOrLock = 0;
			for (int i=0; i<5; i++) {
				cout<<"setting" <<endl;
				heights[i] = 0;
				cout<<"done setting" <<endl;
			}
			continue;
		}
		if (!keyOrLock) {
			if (s == "#####") {
				keyOrLock = 1; // Lock
			} else {
				keyOrLock = -1; // Key
			}
		}
		for (int i=0; i<s.length(); i++) {
			if (s[i] == '#') {
				cout<<"yep"<<endl;
				heights[i]++;
			}
		}
	}
	for (int i=0; i<5; i++) {
		heights[i]--;
	}
	if (keyOrLock == -1) {
		keys.push_back(vector<int>(heights));
	} else {
		locks.push_back(vector<int>(heights));
	}
	file.close();

	for (auto key : keys) {
		for (auto height: key) {
			cout << height << ",";
		}
		cout << endl;
	}
	for (auto lock : locks) {
		for (auto height : lock) {
			cout << height << ",";
		}
		cout << endl;
	}

	int count = 0;
	for (auto key : keys) {
		for (auto lock : locks) {
			bool valid = true;
			for (int i=0; i<5; i++) {
				if (key[i] + lock[i] > 5) {
					valid = false;
					break;
				}
			}
			if (valid) {
				count++;
			}
		}
	}
	cout << "count: " << count << endl;
}
