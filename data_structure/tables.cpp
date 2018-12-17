//
// Created by MAC on 2018/12/1.
//

#include "tables.h"

Tables::Tables() {
    KT = { // 预留关键字表
            "main", "char", "double", "enum", "float", "int", "long", "short", "signed", "struct", "union",
            "unsigned", "void", "for", "do", "while", "break", "continue", "if", "else", "goto",
            "switch", "case", "default ", "return", "auto", "extern", "register", "static", "const",
            "sizeof", "typedef", "volatile", "putc", "puts"
    };
    PT = { // 界符表
            "#", "+", "-", "*", "/", "=", ">", "<", ",", ".", "&", "|", "!", "^", ";", "{", "}", "[",
            "]", ":", "?", "(", ")", "%", //  0 ~ 23 单字符
            "+=", "-=", "*=", "/=", "%=", "<=", ">=", "==", "&=", "|=", "!=", "^=", "^|", "&&", "||",
            "<<", ">>", "//", "/*", "*/", // 24 ~ 43 两字符
            "<<=", ">>=" // 44 ~ 45 三字符
    };
}

Tables::~Tables() = default;

ostream& operator<<(ostream& out, Tables::Number n) {
    switch(n.type) {
        case Tables::INTEGER:
            out << n.value.i;
            break;
        case Tables::FLOAT:
            out << n.value.f;
            break;
        default: break;
    }
    return out;
}

bool operator==(Tables::Number n_1, Tables::Number n_2) {
    if (n_1.type == n_2.type) {
        switch(n_1.type) {
            case Tables::INTEGER:
                return n_1.value.i == n_2.value.i;
            case Tables::FLOAT:
                return n_1.value.f == n_2.value.f;
            default: return false;
        }
    } else return false;
}


