//
// Created by MAC on 2018/12/10.
//

#include "recursub.h"
#include <utility> #include "../lexical_analysis/scanner.h"
#include "../data_structure/tables.h"
#include "../data_structure/token.h"

Recursub::Recursub(Grammar& G, Scanner& sc): G(G), scanner(sc) {
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
                    cout << "Intersactive Select-set:" << iter_1.first << " -> " << iter_2 << endl;
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
    this->token = token;
    if (token.kind == "K" || token.kind == "P") {
        return token.src;
    } else if (token.kind == "I") {
        return "@I";
    } else if (token.kind == "C") {
        return (G.tables.CT[token.index])->type == Tables::INTEGER ? "@INT" : "@FLT";
    } else if (token.kind == "c") {
        return "@CH";
    } else if (token.kind == "S") {
        return "@STR";
    } else return "";
}

bool Recursub::subprogram(string left) {
    auto p = Grammar::G.find(left);

    // 查看以left为左部的产生式中是否包含空，即left是否可以推出空
    bool epsilon_flag = false;
    for (auto rl: p->second) {
        if (rl.size() == 1 && rl.front() == Grammar::null) epsilon_flag = true;
    }

    for (auto rl: p->second) { // 遍历以left为左部的所有产生式
        set<string> select = G.select_set_of(p->first, rl); // 计算产生式的select集
        if (!select.count(w)) continue; // 如果产生式的select集不包含w，则遍历下一个产生式
        Token token_sav;
        for (auto rs: rl) { // 遍历产生右部的每个符号
            if (G.symbol_type(rs) == 1) { // 如果符号为非终结符
                if (!subprogram(rs)) return false;
            } else { // 如果符号为终结符或空或翻译文法符号
                if (rs.find("qua") == 0) {
                    char operat = rs[3];
                    if (operat == 'p') {
                        operands.push_back(G.tables.CT[token_sav.index]);
                    } else if (operat == '.') {
                        auto * res_1 = new Tables::Number;
                        auto * res_2 = operands.back();
                        auto * res = new Tables::Number;
                        res_1->type = Tables::INTEGER;
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
                    continue; // 如果是翻译文法符号，则遍历下一个符号
                }
                if (w == rs) { // 如果是终结符且与token对应字符串匹配
                    token_sav = this->token;
                    try {
                        w = token2str(scanner.scan_next()); // 取下一个token，转为字符串以匹配文法
                    } catch (ScannerException& e) {
                        if (e.get_log() != Errors::fake_error[0]) throw e;
                    }
                } else if (!epsilon_flag) {
                    return false; // 如果都不满足，且left不可推出空，则报错
                }
            }
        }
        break;
    }
    return true;
}


vector<Quarternary> Recursub::check_trans() {
    vector<int> layers = {1};
    vector<Quarternary> error;
    try {
        w = token2str(scanner.scan_next()); // 取下一个token，转为字符串以匹配文法
    } catch (ScannerException& e) {
        if (e.get_log() != Errors::fake_error[0]) throw e;
    }
    if (subprogram(Grammar::S)) {
        return Qs;
    } else throw SyntaxException(0, Errors::syntax_error[3]);
}