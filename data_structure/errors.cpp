//
// Created by MAC on 2018/12/4.
//

#include "errors.h"

const string Errors::syntax_error[] = {
    "Syntax Error: incorrect character", // 0
    "Syntax Error: incorrect character string",
    "Syntax Error: incorrect constant",
    "Syntax Error: wrong expression at or after",
    "Syntax Error: wrong index for array declaration",
    "Syntax Error: redeclaration of variable", // 5
    "Syntax Error: undeclared identifier",
    "Syntax Error: redeclaration of function",
    "Syntax Error: undeclared struct type"
};
const string Errors::symbol_error[] = {
    "Symbol Error: incorrect delimiters",
    "Symbol Error: incorrect annotation"
};
const string Errors::fake_error[] = {
    "EOF"
};
const string Errors::file_error[] = {
    "File Error: failed to open source file"
};