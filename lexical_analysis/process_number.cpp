//
// Created by MAC on 2018/12/4.
//

#include "process_number.h"
#include "../data_structure/tables.h"

P_number::Shifter P_number::shifter[7] = {
        [](char ch, State state)->State {
            if (is_digit(ch)) return state_1;
            else return state_err;
        },
        [](char ch, State state)->State {
            if (is_digit(ch)) return state_1;
            else if (right_suffix(ch)) return state_end;
            else if (ch == '.') return state_2;
            else if (ch == 'e' || ch == 'E') return state_4;
            else return state_err;
        },
        [](char ch, State state)->State {
            if (is_digit(ch)) return state_3;
            else return state_err;
        },
        [](char ch, State state)->State {
            if (is_digit(ch)) return state_3;
            else if (right_suffix(ch)) return state_end;
            else if (ch == 'e' || ch == 'E') return state_4;
            else return state_err;
        },
        [](char ch, State state)->State {
            if (is_digit(ch)) return state_6;
            else if (ch == '+' || ch == '-') return state_5;
            else return state_err;
        },
        [](char ch, State state)->State {
            if (is_digit(ch)) return state_6;
            else return state_err;
        },
        [](char ch, State state)->State {
            if (is_digit(ch)) return state_6;
            else if (right_suffix(ch)) return state_end;
            else return state_err;
        }
};

P_number::P_number(string &bf): buffer(bf) {}

bool P_number::process(size_t &cur_index, Tables::Number* num) {
    State state = state_0; char ch = '\0';
    int value = 0, exp_value = 0, frac_width = 0, exp_sign = 1;
    while (state != state_end) {
        switch (state) {
            case state_0:
                value = exp_value = frac_width = 0;
                exp_sign = 1;
                num->value.i = 0;
                num->type = Tables::INTEGER;
                break;
            case state_1:
                value = 10 * value + (ch - '0');
                break;
            case state_2:
                num->type = Tables::FLOAT;
                break;
            case state_3:
                value = 10 * value + (ch - '0');
                frac_width ++;
                break;
            case state_4:
                num->type = Tables::FLOAT;
                break;
            case state_5:
                if (ch == '-') exp_sign = -1;
                break;
            case state_6:
                exp_value = 10 * exp_value + (ch - '0');
                break;
            case state_err:
                return false;
            default:
                break;
        }
        ch = buffer[cur_index ++];
        state = (*shifter[state])(ch, state);
    }
    if (num->type == Tables::INTEGER)
        num->value.i = static_cast<int>(value * pow(10, exp_sign * exp_value - frac_width));
    else if (num->type == Tables::FLOAT)
        num->value.f = static_cast<float>(value * pow(10, exp_sign * exp_value - frac_width));
    cur_index --;
    return true;
}

bool P_number::right_suffix(char ch) {
    vector <char> allow_suffix = { //算术常数允许的后继符集
        ' ', '\n', '+', '-', '*', '/', '=', '>', '<', ',',
        '&', '|', '!', '^', ']', ':', '?', ')', ';'
    };
    for (auto suffix: allow_suffix) if (ch == suffix) return true;
    return false;
}