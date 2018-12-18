//
// Created by MAC on 2018/12/1.
//

#include "tables.h"

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
    auto * synbl_dict_v = new SYNBL_DICT_V;
    synbl_dict_v->child = synbl;
    if (synbl_cur != nullptr) {
        synbl_dict_v->parent = synbl_cur->child;
        synbl_dict_v->level = synbl_cur->level + 1;
    } else {
        synbl_dict_v->parent = nullptr;
        synbl_dict_v->level = 0;
    }

    // 主表记录填入字典
    synbl_dict.insert(pair<string, SYNBL_DICT_V> (name, *synbl_dict_v));

    // 修改当前主表记录
    synbl_cur = synbl_dict_v;
}

Tables::SYNBL_V *Tables::search(SYNBL* table, string src) {
    for (SYNBL_V *item: *table) if (item->src == src) return item;
    return nullptr;
}

// TODO: value 临时变量可能丢失
void Tables::add(Tables::SYNBL *table, string& src) {
    SYNBL_V* value = new SYNBL_V;
    value->src = src;
    table->push_back(value);
}
