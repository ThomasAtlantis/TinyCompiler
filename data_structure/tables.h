//
// Created by MAC on 2018/12/1.
//

#ifndef COMPILE_TABLES_H
#define COMPILE_TABLES_H

#include "../utility/utility.h"

class Tables {
public:
    typedef enum {INTEGER, FLOAT} Num_type;
    typedef union {float f; int i;} Number;

    vector<string> KT;
    vector<string> PT;
    vector<string> IT;
    vector<char> cT;
    vector<string> ST;
    vector<Number> CT;
    Tables();
    ~Tables();
};

#endif //COMPILE_TABLES_H
