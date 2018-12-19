//
// Created by MAC on 2018/12/4.
//

#ifndef COMPILE_PROCESS_NUMBER_H
#define COMPILE_PROCESS_NUMBER_H

#include "../utility/automaton.h"
#include "../utility/utility.h"
#include "../data_structure/tables.h"

class P_number: public Automaton{
private:
    string& buffer;
    static Shifter shifter[7];

    static bool right_suffix(char ch);

public:
    explicit P_number(string& buffer);

    bool process(size_t& cur_index, Tables::Number* num);
};

#endif //COMPILE_PROCESS_NUMBER_H
