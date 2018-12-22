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
    struct x {
        int i;
    };
    struct x y {};
    y.i = 10;
    cout << y.i << endl;
    return 0;
}