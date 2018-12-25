//
// Created by MAC on 2018/12/3.
//

#include "token.h"
#include "tables.h"

ostream& operator<<(ostream& out, Token token) {
    out << "-> {"
        << "src: " << setw(10) << left << token->src << ", ";
    if (token->cate == Tables::CONSTANT) {
        cout << "type: " << setw(10) << left <<  Tables::get_type_name(token->type->tval);
    } else {
        out << "type: " << setw(10) << left <<  "--";
    }
    out << "cate: " << setw(10) << left <<  Tables::get_cate_name(token->cate)
        << "addr: " << setw(10) << left << "--"
        << "}";
    return out;
}