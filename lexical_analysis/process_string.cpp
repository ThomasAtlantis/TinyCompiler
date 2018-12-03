//
// Created by MAC on 2018/12/4.
//

#include "process_string.h"

P_string::Shifter P_string::shifter[4] = {
        [](char ch, State state)->State {
            if (ch == '\\') return state_1;
            else if (ch == '\n' || ch == '\'') return state_err;
            else if (ch == '\"') return state_end;
            else return state_3;
        },
        [](char ch, State state)->State {
            if (ch == 'n' || ch == 't' || ch == '0' ||
                ch == '\\' || ch == '\'' || ch == '\"')
                return state_2;
            else return state_err;
        },
        [](char ch, State state)->State {
            return state_0;
        },
        [](char ch, State state)->State {
            return state_0;
        }
};

P_string::P_string(string &bf): buffer(bf) {}

bool P_string::process(int &cur_index, string &str) {
    State state = state_0;
    char cur_ch = '\0'; str = "";
    while (state != state_end) {
        switch (state) {
            case state_2:
                char ch;
                switch (cur_ch) {
                    case 'n': ch = '\n'; break;
                    case 't': ch = '\t'; break;
                    case '0': ch = '\0'; break;
                    default : ch = cur_ch; break;
                }
                str += ch;
                cur_index --;
                break;
            case state_3:
                str += cur_ch;
                cur_index --;
                break;
            case state_err:
                return false;
            default:
                break;
        }
        if (cur_index == buffer.length()) return false;
        cur_ch = buffer[cur_index ++];
        state = (*shifter[state])(cur_ch, state);
    }
    return true;
}
