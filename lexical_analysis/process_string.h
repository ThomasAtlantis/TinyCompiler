//
// Created by MAC on 2018/12/4.
//

#ifndef COMPILE_PROCESS_STRING_H
#define COMPILE_PROCESS_STRING_H

#include "../utility/automaton.h"

class P_string: public Automaton{
private:
    string& buffer;
    static Shifter shifter[4];

public:
    explicit P_string(string& buffer);

    bool process(size_t& cur_index, string& str);
};

#endif //COMPILE_PROCESS_STRING_H
