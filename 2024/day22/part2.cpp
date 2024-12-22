#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <tuple>
#include <vector>
#include <format>

using namespace std;

long long getNextNum(long long num) {
	long long toMix = num*64;
	num = toMix ^ num;
	num = num % 16777216;

	toMix = num / 32;
	num = toMix ^ num;
	num = num % 16777216;

	toMix = num * 2048;
	num = toMix ^ num;
	num = num % 16777216;

	return num;
}

string getKey(int p1, int p2, int p3, int p4) {
	string key = to_string(p1);
	key += ", " + to_string(p2);
	key += ", " + to_string(p3);
	key += ", " + to_string(p4);
	return key;
}

int main(int argc, char **argv) {
	string s;
	ifstream file(argv[1]);

	vector<long long> nums;
	while (getline(file, s)) {
		nums.push_back(stol(s));
		cout << s << endl;
	}
	file.close();


	//
	map<string, int> maxCosts;
	long long sum = 0;
	for (int i=0; i<nums.size(); i++) {
		map<string, int> costs;
		long long num = nums[i];

		long long lastPrice = num % 10;
		long long p1;
		long long p2;
		long long p3;
		long long p4;
		for (int j=0; j<2000; j++) {
			num = getNextNum(num);
			long long curPrice = num % 10;
			long long diff = curPrice - lastPrice;
			if (j == 0) {
				p1 = diff;
			} else if (j == 1) {
				p2 = diff;
			} else if (j == 2) {
				p3 = diff;
			} else if (j == 3) {
				p4 = diff;
				string key = getKey(p1, p2, p3, p4);
				if (costs.find(key) == costs.end()) {
					costs[key] = curPrice;
				}

			} else {
				p1 = p2;
				p2 = p3;
				p3 = p4;
				p4 = diff;
				string key = getKey(p1, p2, p3, p4);
				if (costs.find(key) == costs.end()) {
					costs[key] = curPrice;
				}
			}
			lastPrice = curPrice;
		}
		map<string, int>::iterator it;
		for (it = costs.begin(); it != costs.end(); ++it) {
			maxCosts[it->first] += it->second;
		}
		
		sum += num;
	}

	map<string, int>::iterator it = maxCosts.begin();
	long long most = 0;
	string key;
	for (; it != maxCosts.end(); ++it) {
		if (it->second > most) {
			key = it->first;
			most = it->second;
		}
	}

	cout << "key: " << key << endl;
	cout << "most: " << most << endl;
}
