//
// Created by MAC on 2018/12/10.
//

#include "quarternary.h"

void print_qs(vector<Quarternary>& Qs) {
    for (auto &Q : Qs) {
        cout << "(" << setw(8) << left << Q.op << ", ";
        if (Q.a != nullptr) cout << setw(8) << left << (Q.a)->src << ", ";
        else cout << setw(8) << left << "--" << ", ";
        if (Q.b != nullptr) cout << setw(8) << left << (Q.b)->src << ", ";
        else cout << setw(8) << left << "--" << ", ";
        if (Q.res != nullptr) cout << setw(8) << left << (Q.res)->src << ")" << endl;
        else cout << setw(8) << left << "--" << ")" << endl;
    }
}