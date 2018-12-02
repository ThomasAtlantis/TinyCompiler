//
// Created by MAC on 2018/12/1.
//

#ifndef COMPILE_GRAMMAR_H
#define COMPILE_GRAMMAR_H

#include "../utility/utility.h"
#include "tables.h"

class Grammar {
public:
    // 类内类型定义
    typedef vector<string> Right_symbols;
    typedef vector<Right_symbols> Right_lists;
    typedef map<string, Right_lists> Productions;
    typedef struct ProductionAdder{
        string left;
        ProductionAdder& operator >>(string right);
        ProductionAdder& operator |(string right);
    } ProductionAdder;
    typedef enum {VT=-1, Null=0, VN=1} symbol_t;

    // 类的成员变量
    static Productions G;
    static string S;
    static string null;
    static string bound;
    static map<string, set<string>> firsts;
    static map<string, set<string>> follows;
    Tables& tables;

    // 构造函数和析构函数
    explicit Grammar(Tables& t);
    ~Grammar();

    // 添加文法产生式，格式 .add(<left>) >> right1 | right2 | ...
    ProductionAdder add(string left);

    // 设置文法开始符号
    void set_start(string& start);

    // 判断符号类型：终结符、非终结符、空字符
    symbol_t symbol_type(string& x);

    // 重载输出函数
    friend ostream & operator<<(ostream &out, Grammar& grammar);

    // 求x的首符集
    set<string> first_set_of(Right_symbols x);

    // 求x的后继集
    set<string> follow_set_of(string x);

    // 求产生式left -> right的选择集
    set<string> select_set_of(string left, Right_symbols right);
};

#endif //COMPILE_GRAMMAR_H

