//
// Created by MAC on 2018/12/1.
//

#ifndef COMPILE_TABLES_H
#define COMPILE_TABLES_H

#include "../utility/utility.h"

class Tables {
public:
    typedef enum {INTEGER, FLOAT} Num_type;
    typedef struct {
        Num_type type;
        union {
            float f;
            int i;
        } value;
    } Number;
    typedef char Charac;

    vector<string> KT;
    vector<string> PT;
    vector<string> IT;
    vector<Charac> cT;
    vector<string> ST;
    vector<Number*> CT;
    Tables();
    ~Tables();
};

ostream& operator<<(ostream& out, Tables::Number n);
bool operator==(Tables::Number n_1, Tables::Number n_2);

#endif //COMPILE_TABLES_H
