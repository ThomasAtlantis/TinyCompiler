//
// Created by MAC on 2018/12/1.
//

#include "tables.h"
#include <utility>

size_t Tables::global_count = 0;

Tables::Tables() {
    KT = { // 预留关键字表
            "main", "char", "double", "enum", "float", "int", "long", "short", "signed", "struct", "union",
            "unsigned", "void", "for", "do", "while", "break", "continue", "if", "else", "goto",
            "switch", "case", "default ", "return", "auto", "extern", "register", "static", "const",
            "sizeof", "typedef", "volatile", "putc", "puts", "bool"
    };
    PT = { // 界符表
            "#", "+", "-", "*", "/", "=", ">", "<", ",", ".", "&", "|", "!", "^", ";", "{", "}", "[",
            "]", ":", "?", "(", ")", "%", //  0 ~ 23 单字符
            "+=", "-=", "*=", "/=", "%=", "<=", ">=", "==", "&=", "|=", "!=", "^=", "^|", "&&", "||",
            "<<", ">>", "//", "/*", "*/", // 24 ~ 43 两字符
            "<<=", ">>=" // 44 ~ 45 三字符
    };
    synbl_cur = nullptr;
    new_synbl("#GLOBAL");
}

Tables::~Tables() = default;

ostream& operator<<(ostream& out, Tables::Number n) {
    switch(n.type) {
        case Tables::INTEGER:
            out << n.value.i;
            break;
        case Tables::FLOAT:
            out << n.value.f;
            break;
        default: break;
    }
    return out;
}

bool operator==(Tables::Number n_1, Tables::Number n_2) {
    if (n_1.type == n_2.type) {
        switch(n_1.type) {
            case Tables::INTEGER:
                return n_1.value.i == n_2.value.i;
            case Tables::FLOAT:
                return n_1.value.f == n_2.value.f;
            default: return false;
        }
    } else return false;
}

void Tables::new_synbl(string name) {
    // 新建主表
    auto * synbl = new SYNBL;

    // 主表记录
    if (synbl_cur != nullptr) {
        synbl->parent = synbl_cur;
        synbl->level = synbl_cur->level + 1;
        synbl->index = synbl_cur->content.size();
        if (synbl->index != 0) synbl->index --;
    } else {
        synbl->parent = nullptr;
        synbl->level = 0;
    }

    // 主表记录填入字典
    synbl_dict.insert(pair<string, SYNBL*> (name, synbl));

    // 修改当前主表记录
    synbl_cur = synbl;
}

Tables::SYNBL_V *SYNBL::search(const string &src) {
    for (Tables::SYNBL_V *item: content) if (item->src == src) return item;
    return nullptr;
}

string Tables::get_global_name() {
    string global_name = "t" + to_string(global_count);
    global_count += 1;
    return std::move(global_name);
}

size_s Tables::get_size_of(Tables::TVAL type) {
    switch (type) {
        case FLOAT: return 4;
        case INTEGER: return 4;
        case BOOLEAN: return 1;
        case CHAR: return 1;
        default: return 0;
    }
}

Tables::SYNBL_V *Tables::search(const string &src) {
    SYNBL* synbl = synbl_cur;
//    if (!synbl->content.empty()) {
//        for (size_t i = 0; i < synbl->content.size() - 1; i++) {
//            if (synbl->content[i]->src == src) return synbl->content[i];
//        }
//    }
    while (synbl->parent != nullptr) {
        size_t index = synbl->index;
        synbl = synbl->parent;
        for (size_t i = 0; i <= index; i++) { // <= 考虑函数的递归调用
            if (synbl->content[i]->src == src) return synbl->content[i];
        }
    }
    return nullptr;
}

SYNBL* Tables::search_func(const string &src) {
    auto x = synbl_dict.find(src);
    if (x != synbl_dict.end()) return x->second;
    return nullptr;
}

void Tables::ret_synbl() {
    synbl_cur = synbl_cur->parent;
}

SYNBL::SYNBL() {
    auto * rtp = new Tables::TYPEL {Tables::FLOAT, nullptr};
    auto * itp = new Tables::TYPEL {Tables::INTEGER, nullptr};
    auto * btp = new Tables::TYPEL {Tables::BOOLEAN, nullptr};
    auto * ctp = new Tables::TYPEL {Tables::CHAR, nullptr};
    auto * stp = new Tables::TYPEL {Tables::STRING, nullptr};
    typel = {rtp, itp, btp, ctp, stp};
}

Tables::TYPEL *SYNBL::get_xtp(char kind) {
    switch (kind) {
        case 'r': return typel[0];
        case 'i': return typel[1];
        case 'b': return typel[2];
        case 'c': return typel[3];
        case 's': return typel[4];
        default: return nullptr;
    }
}

Tables::TYPEL *SYNBL::get_xtp(Tables::TVAL kind) {
    switch (kind) {
        case Tables::FLOAT: return typel[0];
        case Tables::INTEGER: return typel[1];
        case Tables::BOOLEAN: return typel[2];
        case Tables::CHAR: return typel[3];
        case Tables::STRING: return typel[4];
        default: return nullptr;
    }
}

Tables::SYNBL_V *SYNBL::add(string src) {
    auto * synbl_v = new Tables::SYNBL_V {
        std::move(src), nullptr, Tables::VARIABLE, nullptr
    };
    content.push_back(synbl_v);
    return synbl_v;
}

SYNBL::~SYNBL() {
    for (auto i: content) delete i; content.clear();
    for (auto i: typel) delete i; typel.clear();
    for (auto i: ainel) delete i; ainel.clear();
    for (auto i: pfinfl) delete i; pfinfl.clear();
    for (auto i: param) delete i; param.clear();
}

int find(vector<Tables::SYNBL_V*> values, Tables::SYNBL_V* key) {
    for (size_s index = 0; index < values.size(); index++)
        if (values[index] == key) return index;
    return -1;
}

int find(vector<string*> vec, string key) {
    for (size_s index = 0; index < vec.size(); index++)
        if (*vec[index] == key) return index;
    return -1;
}

int find(vector<Charac*> vec, Charac key) {
    for (size_s index = 0; index < vec.size(); index++)
        if (*vec[index] == key) return index;
    return -1;
}

int find(vector<Tables::Number*> nums, Tables::Number key) {
    for (size_s index = 0; index < nums.size(); index++)
        if (nums[index]->type == key.type) {
            if (key.type == Tables::INTEGER and nums[index]->value.i == key.value.i) return index;
            else if (key.type == Tables::FLOAT and nums[index]->value.f == key.value.f) return index;
            else if (key.type == Tables::BOOLEAN and nums[index]->value.b == key.value.b) return index;
        }
    return -1;
}
