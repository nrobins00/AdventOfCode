#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

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
	long long sum = 0;
	for (int i=0; i<nums.size(); i++) {
		long long num = nums[i];
		cout << "starting with " << num << endl;

		for (int j=0; j<2000; j++) {
			num = getNextNum(num);
		}
		sum += num;
		cout << "ending with " << num << endl;
	}

	cout << "sum: " << sum << endl;
}
