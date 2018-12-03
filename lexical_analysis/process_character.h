//
// Created by MAC on 2018/12/3.
//

#ifndef COMPILE_PROCESS_CHARACTER_H
#define COMPILE_PROCESS_CHARACTER_H

#include "../utility/automaton.h"

class P_charac: public Automaton{
private:
    string& buffer;
    static Shifter shifter[4];

public:
    explicit P_charac(string& buffer);

    bool process(int& cur_index, char& ch);
};

#endif //COMPILE_PROCESS_CHARACTER_H
