//
// Created by MAC on 2018/12/13.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#define all(x) (x).begin(),(x).end()
using namespace std;
int main() {
    vector<string> vec = {"qua+", "qua-", "A", "B", "C"};
    vector<string> right(vec.size());
    auto right_it = copy_if(all(right), right.begin(), [](string& symbol) {
        return symbol.find("qua") == string::npos;
    });
    //right.resize(static_cast<size_t>(distance(right.begin(), right_it)));
    for (auto& x: vec) cout << x << " "; cout << endl;
    for (auto& x: right) cout << x << " "; cout << endl;
    return 0;
}