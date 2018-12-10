//
// Created by MAC on 2018/12/4.
//

#ifndef COMPILE_ERRORS_H
#define COMPILE_ERRORS_H

#include "../utility/utility.h"

class Errors {
public:
    typedef enum {error=-1, clear=0, eof=1} Type;

    typedef struct {
        Type type;
        string log;
    } Error_message;

    static const string syntax_error[];

    static const string symbol_error[];
};

#endif //COMPILE_ERRORS_H
