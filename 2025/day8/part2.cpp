#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <set>
#include <cmath>
#include <limits>

using namespace std;

struct Point {
    long x,y,z;

    Point(long x_val = 0, long y_val = 0, long z_val = 0) : x(x_val), y(y_val), z(z_val) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

float calcDist(Point p1, Point p2) {
    return sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)* (p2.y-p1.y) + (p2.z - p1.z)*(p2.z - p1.z));
}

int main(int argc, char **argv) {
    ifstream file(argv[1]);
    
    string line;
    vector<Point> points;
    vector<vector<Point>> junctions;
    while (getline(file, line)) {
        istringstream iss(line);
        string x,y,z;
        getline(iss, x, ',');
        getline(iss, y, ',');
        getline(iss, z, ',');
        cout << x << " " << y << " " << z << endl;
        Point newPoint = Point{stoi(x),stoi(y),stoi(z)};
        points.push_back(newPoint);
        vector<Point> newSet;
        newSet.push_back(newPoint);
        junctions.push_back(newSet);
    }
    float smallestDist;
    float lastSmallestDist = 0;
    Point p1, p2;
    while (junctions.size() > 1) {
        smallestDist = numeric_limits<float>::max();
        for (int i=0; i<points.size()-1; i++) {
            for (int j=i+1; j<points.size(); j++) {
                float dist = calcDist(points[i], points[j]);
                if (dist < smallestDist && dist > lastSmallestDist) {
                    smallestDist = dist;
                    p1 = points[i];
                    p2 = points[j];
                }
            }
        }
        lastSmallestDist = smallestDist;
        //cout << "lastsmallestdist: " << lastSmallestDist << endl;
        
        //cout << "now doing: " << p1.x << ", " << p1.y << ", " << p1.z << " & " << p2.x << ", " << p2.y << ", " << p2.z << endl;
        int firstJunction = -1;
        int secondJunction = -1;
        for (int r=0;r<junctions.size(); r++) {
            for (int r2=0; r2<junctions[r].size(); r2++) {
                if (junctions[r][r2] == p1) {
                    firstJunction = r;
                }
                if (junctions[r][r2] == p2) {
                    secondJunction = r;
                }
            }
        }
        if (firstJunction == secondJunction) { continue; }
        for (auto it = junctions[secondJunction].begin(); it != junctions[secondJunction].end(); it++) {
            junctions[firstJunction].push_back(*it);
        }
        junctions.erase(junctions.begin() + secondJunction);
    }
    cout << p1.x * p2.x << endl;
    return 0;
    vector<long> sizes;
    for (vector<Point> s : junctions) {
        // for (Point p : s) {
        //     cout << "(" <<p.x << "," <<p.y <<"," <<p.z <<"), ";
        // }
        // cout <<endl;
        cout << s.size() << endl;
        sizes.push_back(s.size());
    }
    sort(sizes.begin(), sizes.end(), greater<int>());
    cout << sizes[0] * sizes[1] * sizes[2] << endl;
} 