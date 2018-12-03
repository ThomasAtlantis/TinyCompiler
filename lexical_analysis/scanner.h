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

typedef struct {
    Token token;
    int error_type;
    string error_log;
} ScannerGet;

class Scanner {
private:
    string& buffer;
    Tables& tables;
    static int curr_index;
    static int line_label;
public:
    P_string p_string;
    P_charac p_charac;
    P_number p_number;

    Scanner(string& bf, Tables& tbs);
    ~Scanner();

    ScannerGet scan_next();
};


#endif //COMPILE_SCANNER_H
