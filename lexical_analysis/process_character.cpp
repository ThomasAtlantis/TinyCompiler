//
// Created by MAC on 2018/12/3.
//

#include "process_character.h"

P_charac::Shifter P_charac::shifter[4] = {
        [](char ch, State state)->State {
            if (ch == '\\') return state_1;
            else if (ch == '\n' || ch == '\'' || ch == '\"') return state_err;
            else return state_3;
        },
        [](char ch, State state)->State {
            if (ch == 'n' || ch == 't' || ch == '0' ||
                ch == '\\' || ch == '\'' || ch == '\"')
                return state_2;
            else return state_err;
        },
        [](char ch, State state)->State {
            if (ch == '\'') return state_end;
            else return state_err;
        },
        [](char ch, State state)->State {
            if (ch == '\'') return state_end;
            else return state_err;
        }
};

P_charac::P_charac(string& bf):buffer(bf) {}

bool P_charac::process(int &cur_index, char &ch) {
    State state = state_0;
    char cur_ch = '\0';
    while (state != state_end) {
        switch (state) {
            case state_2:
                switch (cur_ch) {
                    case 'n': ch = '\n'; break;
                    case 't': ch = '\t'; break;
                    case '0': ch = '\0'; break;
                    default : ch = cur_ch; break;
                }
                break;
            case state_3:
                ch = cur_ch;
                break;
            case state_err:
                return false;
            default:
                break;
        }
        cur_ch = buffer[cur_index ++];
        state = (*shifter[state])(cur_ch, state);
    }
    return true;
}
