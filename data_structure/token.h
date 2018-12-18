//
// Created by MAC on 2018/12/3.
//

#ifndef COMPILE_TOKEN_H
#define COMPILE_TOKEN_H

#include "../utility/utility.h"

typedef struct {
    string kind;
    size_t index;
    string src;
} Token;

ostream& operator<<(ostream& out, Token token);

#endif //COMPILE_TOKEN_H
