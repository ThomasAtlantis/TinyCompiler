//
// Created by MAC on 2018/12/12.
//

#ifndef COMPILE_TABLES_H
#define COMPILE_TABLES_H

#include "../utility/utility.h"
/*
class Tables {
public:
    typedef enum {INTEGER, FLOAT, BOOLEAN, STRUCTURE, ARRAY} TVAL;
    typedef enum {DOMAINN, FUNCTION, CONSTANT, TYPE, VARIABLE} CATE;
    typedef unsigned int size_s;
    typedef struct TYPEL TYPEL;
    typedef struct AINEL AINEL;
    typedef struct RINEL RINEL;
    typedef struct PFINFL PFINFL;
    typedef struct SYNBL_V SYNBL_V;
    typedef struct vector<size_t> LENL;
    typedef struct VALL VALL;
    typedef struct PARAM PARAM;

    typedef union {
        string str;
        char ch;
        int i;
        float f;
        bool b;
    } Constant; // 常量类型

    typedef vector<Constant> CONSL; // 常量表

    struct AINEL {
        size_t low;
        size_t up;
        size_t clen;
        TYPEL* ctp;
    }; // 数组信息表

    struct RINEL {
        string id;
        size_t off;
        TYPEL* tp;
        RINEL* next;
    }; // 结构体信息表

    struct PARAM {
        string id;
        TYPEL* type;
        CATE cate;
        void* addr;
        PARAM* next;
    }; // 函数形参表

    struct PRINFL {
        size_s level;
        size_t off;
        size_s fn;
        size_t entry;
        PARAM* param;
    }; // 函数信息表

    struct TYPEL {
        TVAL tval;
        union {
            AINEL* a;
            RINEL* r;
        } tptr;
    }; // 类型表

    struct SYNBL_V {
        TYPEL* type;
        CATE cate;
        void* addr;
    }; // 符号表主表值

    typedef map<string, SYNBL_V> SYNBL; // 符号表主表，以单词源码为索引的字典

    SYNBL ST; // 符号表主表
    vector<string> KT; // 关键字表
    vector<string> PT; // 界符表

    Tables();
    ~Tables();
};

typedef struct {
    bool synbl; // 是符号表符号则为真，否则为假
    string src; // 填写单词源码，当synbl为真时查填符号表，否则直接匹配文法
} Token; // 词法分析器返回Token
*/
#endif //COMPILE_TABLES_H
