//
// Created by MAC on 2018/12/10.
//

#include "recursub.h"

Recursub::Recursub(Grammar& G): G(G) {
    available = is_available();
}

Recursub::~Recursub() = default;

bool Recursub::is_available() {
    for (auto iter_1: Grammar::G) {
        if (iter_1.second.size() > 1) {
            set<string> s;
            for (auto iter_2: iter_1.second) {
                if (iter_2.front() == iter_1.first) {
                    cout << "Left Recursion" << endl;
                    return false;
                }
                set<string> tmp = G.select_set_of(iter_1.first, iter_2);
                size_t old_size = s.size();
                s.insert(tmp.begin(), tmp.end());
                if (s.size() < old_size + tmp.size()) {
                    cout << "Intersactive Select-set" << endl;
                    return false;
                }
            }
        } else if (iter_1.second[0][0] == iter_1.first) {
            cout << "Left Recursion" << endl;
            return false;
        }
    }
    return true;
}

string Recursub::token2str(Token token) {
    string w;
    switch (token.kind) {
        case 'K': w = G.tables.KT[token.index]; break;
        case 'P': w = G.tables.PT[token.index]; break;
        case 'I': w = "/I"; break;
        case 'C': w = "/C"; break;
        case 'c': w = G.tables.cT[token.index]; break;
        case 'S': w = G.tables.ST[token.index]; break;
        case '#': w = "#"; break;
        default: break;
    }
    return w;
}

// TODO: get token from scanner.scan_next() in subprogram

bool Recursub::subprogram(
    vector<Token>& tokens,
    string left,
    vector<Quarternary>& Qs,
    vector<Tables::Number*>& operands,
    vector<int>& layers
) {
    auto p = Grammar::G.find(left);
    bool epsilon_flag = false;
    for (auto rl: p->second) {
        if (rl.size() == 1 && rl.front() == "epsilon") epsilon_flag = true;
    }
    for (auto rl: p->second) {
        set<string> select = G.select_set_of(p->first, rl);
        string w = token2str(tokens.front());
        if (!select.count(w)) continue;
        Token token_sav;
        for (auto rs: rl) {
            if (G.symbol_type(rs) == 1) {
                if (!subprogram(tokens, rs, Qs, operands, layers)) return false;
                w = token2str(tokens.front());
            } else {
                if (rs.find("qua") == 0) {
                    char operat = rs[3];
                    if (operat == 'p') {
                        operands.push_back(&G.tables.CT[token_sav.index]);
                    } else if (operat == '.') {
                        auto * res_1 = new Tables::Number;
                        auto * res_2 = operands.back();
                        auto * res = new Tables::Number;
                        res_1->value.i = 0;
                        operands.pop_back();
                        Quarternary Q = {'-', res_1, res_2, res};
                        Qs.push_back(Q);
                        operands.push_back(res);
                    } else {
                        auto * res_2 = operands.back();
                        operands.pop_back();
                        auto * res_1 = operands.back();
                        operands.pop_back();
                        auto * res = new Tables::Number;
                        Quarternary Q = {operat, res_1, res_2, res};
                        Qs.push_back(Q);
                        operands.push_back(res);
                    }
                    continue;
                }
                if (w == rs) {
                    token_sav = tokens.front();
                    tokens.erase(tokens.begin());
                } else if (!epsilon_flag) return false;
            }
        }
        break;
    }
    return true;
}


vector<Quarternary> Recursub::check_trans(vector<Token> tokens) {
    Token token = {'#', 0};
    tokens.push_back(token);
    vector<int> layers = {1};
    vector<Quarternary> Qs;
    vector<Quarternary> error;
    vector<Tables::Number*> operands;
    if (subprogram(tokens, Grammar::S, Qs, operands, layers)) {
        return Qs;
    } else {
        cout << "Syntax Error(?): wrong experession" << endl;
        return error;
    }
}