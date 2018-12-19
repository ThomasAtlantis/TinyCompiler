//
// Created by MAC on 2018/12/10.
//

#include "quarternary.h"

void print_qs(vector<Quarternary>& Qs) {
    for (auto &Q : Qs) {
        cout << "(" << setw(8) << left << Q.op << ", ";
        cout << setw(8) << left << (Q.a)->src << ", ";
        cout << setw(8) << left << (Q.b)->src << ", ";
        cout << (Q.res)->src << ")" << endl;
    }
}