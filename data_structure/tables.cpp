//
// Created by MAC on 2018/12/1.
//

#include "tables.h"

Tables::Tables() {
    KT = { // 预留关键字表
            "char", "double", "enum", "float", "int", "long", "short", "signed", "struct", "union",
            "unsigned", "void", "for", "do", "while", "break", "continue", "if", "else", "goto",
            "switch", "case", "default ", "return", "auto", "extern", "register", "static", "const",
            "sizeof", "typedef", "volatile"
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


