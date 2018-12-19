//
// Created by MAC on 2018/12/10.
//

#include "quarternary.h"

void print_qs(vector<Quarternary>& Qs) {
    vector<Tables::SYNBL_V*> points;
    for (auto &Q : Qs) {
        cout << "(" << setw(8) << left << Q.op << ", ";
        int it_1 = find(points, Q.a);
        int it_2 = find(points, Q.b);
        if (it_1 == -1) cout << setw(8) << left << (Q.a)->src << ", ";
        else cout << "t" << setw(7) << left << it_1 << ", ";
        if (it_2 == -1) cout << setw(8) << left << (Q.b)->src << ", ";
        else cout << "t" << setw(7) << left << it_2 << ", ";
        points.push_back(Q.res);
        cout << "t" << setw(7) << left << points.size() - 1 << ")" << endl;
    }
}