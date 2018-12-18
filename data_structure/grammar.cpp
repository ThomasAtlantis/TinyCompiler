//
// Created by MAC on 2018/12/1.
//

#include "grammar.h"

Grammar::Productions Grammar::G;
string Grammar::S;
string Grammar::null = "null";
string Grammar::bound = "#";
map<string, set<string>> Grammar::firsts;
map<string, set<string>> Grammar::follows;
vector<string> Grammar::todos;

Grammar::Grammar(Tables &t): tables(t) {}

Grammar::~Grammar() = default;

// 设置文法开始符号`开始符号
void Grammar::set_start(string start) {
    Grammar::S = std::move(start);
}

Grammar::ProductionAdder Grammar::add(string left) {
    Grammar::ProductionAdder productionAdder = {std::move(left)};
    return productionAdder;
}

Grammar::ProductionAdder &Grammar::ProductionAdder::operator >>(const string right) {
    auto iter = G.find(left);
    if (iter != G.end()) iter->second.push_back(split(right));
    else G.insert(pair<string, Right_lists>(left, {split(right)}));
    return *this;
}

Grammar::ProductionAdder &Grammar::ProductionAdder::operator|(const string right) {
    return operator>>(right);
}

Grammar::symbol_t Grammar::symbol_type(string &x) {
    if (x == null) return Null;
    else if (G.find(x) == G.end()) return VT;
    else return VN;
}

ostream &operator<<(ostream &out, Grammar &grammar) {
    for (auto production: Grammar::G) {
        out << production.first << " -> ";
        for (const auto &right: production.second) {
            for (const auto &symbol: right) out << symbol << " ";
            if (right != *(production.second.end() - 1)) out << "| ";
        }
        out << endl;
    }
    return out;
}

set<string> Grammar::first_set_of(Grammar::Right_symbols x) {
    set<string> first_set;
    Right_lists right_lists;
    if (x.size() == 1) {
        if (symbol_type(x[0]) != VN) {
            first_set.insert(x[0]);
            return first_set;
        }
        auto it = firsts.find(x[0]);
        if (it != firsts.end()) return it->second;
        right_lists = (*G.find(x[0])).second;
    } else right_lists.push_back(x);
    for (auto right: right_lists) {
        string f = right.front();
        if (symbol_type(f) == VN) {
            Right_symbols::iterator symbol;
            for (symbol = right.begin(); symbol != right.end(); symbol ++) {
                set<string> tmp = first_set_of(split(*symbol));
                if (tmp.count(null)) {
                    tmp.erase(null);
                    first_set.insert(tmp.begin(), tmp.end());
                } else {
                    first_set.insert(tmp.begin(), tmp.end());
                    break;
                }
            }
            if (symbol == right.end()) first_set.insert(null);
        } else first_set.insert(f);
    }
    if (x.size() == 1 && !first_set.empty()) firsts.insert(pair<string, set<string>>(x[0], first_set));
    return first_set;
}

set<string> Grammar::follow_set_of(string x) {
    if (symbol_type(x) != VN) return set<string>();
    auto it = follows.find(x);
    if (it != follows.end()) return it->second;
    set<string> follow_set;
    if (x == S) follow_set.insert(bound);
    for (const auto &production: G) {
        for (auto right: production.second) {
            Right_symbols::iterator where_x_is;
            for (where_x_is = right.begin(); where_x_is != right.end(); where_x_is ++)
                if (*where_x_is == x) break;
            set<string> first;
            if (where_x_is == right.end()) continue;
            if (where_x_is != right.end() - 1) {
                Right_symbols right_of_x(where_x_is + 1, right.end());
                first = first_set_of(right_of_x);
                follow_set.insert(first.begin(), first.end());
                if (first.count(null)) follow_set.erase(null);
            }
            if (production.first != x && (where_x_is == right.end() - 1 || first.count(null))) {

                if (find(all(todos), x) != todos.end()) continue;
                todos.push_back(x);

                set<string> follow = follow_set_of(production.first);

                todos.pop_back();

                follow_set.insert(follow.begin(), follow.end());
            }
        }
    }
    if (!follow_set.empty()) follows.insert(pair<string, set<string>>(x, follow_set));
    return follow_set;
}

set<string> Grammar::select_set_of(string left, Grammar::Right_symbols right) {
    set<string> select = first_set_of(std::move(right));
    if (select.count(null)) {
        select.erase(null);
        set<string> follow = follow_set_of(std::move(left));
        select.insert(follow.begin(), follow.end());
    }
    return select;
}
