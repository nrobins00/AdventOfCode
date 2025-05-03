#include <iostream>
#include <map>

void modifyMap(std::map<int, int>& myMap) {
    myMap[1] = 100;
}

int main() {
    std::map<int, int> myMap = {{1, 2}, {2, 4}};

    modifyMap(myMap);

    std::cout << myMap[1] << std::endl; // Output: 100

    return 0;
}
