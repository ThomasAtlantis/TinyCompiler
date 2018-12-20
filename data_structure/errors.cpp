//
// Created by MAC on 2018/12/4.
//

#include "errors.h"

const string Errors::syntax_error[] = {
    "Syntax Error: incorrect character",
    "Syntax Error: incorrect character string",
    "Syntax Error: incorrect constant",
    "Syntax Error: wrong expression",
    "Syntax Error: wrong index for array declaration",
    "Syntax Error: redeclaration of variable",
    "Syntax Error: undeclared variable"
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