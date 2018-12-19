//
// Created by MAC on 2018/12/10.
//

#ifndef COMPILE_QUARTERNARY_H
#define COMPILE_QUARTERNARY_H

#include "../utility/utility.h"
#include "tables.h"

typedef struct {
    string op;
    Tables::SYNBL_V *a;
    Tables::SYNBL_V *b;
    Tables::SYNBL_V *res;
} Quarternary;

void print_qs(vector<Quarternary>& Qs);

#endif //COMPILE_QUARTERNARY_H
