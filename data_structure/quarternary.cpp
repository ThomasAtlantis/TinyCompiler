//
// Created by MAC on 2018/12/10.
//

#include "quarternary.h"

int find(vector<Tables::Number*> nums, Tables::Number* key) {
    for (unsigned int index = 0; index < nums.size(); index++)
        if (nums[index] == key) return index;
    return -1;
}

void print_qs(vector<Quarternary>& Qs) {
    vector<Tables::Number*> points;
    for (auto &Q : Qs) {
        cout << "(" << setw(8) << left << Q.op << ", ";
        int it_1 = find(points, Q.a);
        int it_2 = find(points, Q.b);
        if (it_1 == -1) cout << setw(8) << left << *(Q.a) << ", ";
        else cout << "t" << setw(7) << left << it_1 << ", ";
        if (it_2 == -1) cout << setw(8) << left << *(Q.b) << ", ";
        else cout << "t" << setw(7) << left << it_2 << ", ";
        points.push_back(Q.res);
        cout << "t" << setw(7) << left << points.size() - 1 << ")" << endl;
    }
}