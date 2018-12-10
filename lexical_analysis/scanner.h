//
// Created by MAC on 2018/12/2.
//

#ifndef COMPILE_SCANNER_H
#define COMPILE_SCANNER_H

#include "../data_structure/data_structure.h"
#include "../utility/utility.h"
#include "process_character.h"
#include "process_number.h"
#include "process_string.h"

class Scanner {
private:
    string& buffer;
    Tables& tables;
    static int curr_index;
    static int line_label;
public:
    typedef struct {
        Errors::Error_message error_m;
        Token token;
    } Scanner_ret;

    P_string p_string;
    P_charac p_charac;
    P_number p_number;

    Scanner(string& bf, Tables& tbs);
    ~Scanner();

    Scanner_ret scan_next();
};


#endif //COMPILE_SCANNER_H
