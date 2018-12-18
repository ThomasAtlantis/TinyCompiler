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
#include "../data_structure/errors.h"

class Scanner {
private:
    string& buffer;
    Tables& tables;
    static size_t curr_index;
    static size_s line_label;
public:

    P_string p_string;
    P_charac p_charac;
    P_number p_number;

    Scanner(string& bf, Tables& tbs);
    ~Scanner();

    Token scan_next();
    size_s get_line();
};


#endif //COMPILE_SCANNER_H
