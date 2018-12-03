//
// Created by MAC on 2018/12/3.
//

#ifndef COMPILE_AUTOMATON_H
#define COMPILE_AUTOMATON_H

#include <iostream>
using namespace std;

class Automaton {
public:
    typedef enum {
        state_0, state_1, state_2, state_3, state_4,
        state_5, state_6, state_7, state_8, state_9,
        state_err, state_end
    } State;

    typedef State(*Shifter)(char, State);

    Automaton() = default;

    ~Automaton() = default;

    bool process(){return true;};
};


#endif //COMPILE_AUTOMATON_H
