//
// Created by MAC on 2018/12/3.
//

#include "token.h"

ostream& operator<<(ostream& out, Token token) {
    out << "{src: " << token->src << ", ...}";
    return out;
}