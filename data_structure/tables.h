//
// Created by MAC on 2018/12/1.
//

#ifndef COMPILE_TABLES_H
#define COMPILE_TABLES_H

#include "../utility/utility.h"

typedef unsigned int size_s;
typedef char Charac;

class Tables {
public:

    typedef enum {INTEGER, FLOAT, BOOLEAN, STRUCTURE, ARRAY} TVAL;
    typedef enum {DOMAINN, FUNCTION, CONSTANT, TYPE, VARIABLE} CATE;
    typedef struct typel TYPEL;
    typedef struct ainel AINEL;
    typedef struct rinel RINEL;
    typedef struct pfinfl PFINFL;
    typedef struct vector<size_t> LENL;
    typedef struct vall VALL;
    typedef struct param PARAM;

    struct ainel {
        size_t low;
        size_t up;
        size_t clen;
        TYPEL* ctp;
    }; // 数组信息表

    struct rinel {
        string id;
        size_t off;
        TYPEL* tp;
        RINEL* next;
    }; // 结构体信息表

    struct param {
        string id;
        TYPEL* type;
        CATE cate;
        void* addr;
        PARAM* next;
    }; // 函数形参表

    struct pfinfl {
        size_s level;
        size_t off;
        size_s fn;
        size_t entry;
        PARAM* param;
    }; // 函数信息表

    struct typel {
        TVAL tval;
        union {
            AINEL* a;
            RINEL* r;
        } tptr;
    }; // 类型表

    typedef struct {
        string src;
        TYPEL* type;
        CATE cate;
        void* addr;
    } SYNBL_V; // 符号表主表值

    typedef vector<SYNBL_V*> SYNBL; // 符号表主表，以单词源码为索引的字典

    typedef struct {
        TVAL type;
        union {
            float f;
            int i;
            bool b;
        } value;
    } Number;

    typedef struct {
        SYNBL* child;
        SYNBL* parent;
        size_t index;
        size_s level;
    } SYNBL_DICT_V;

    map<string, SYNBL_DICT_V> synbl_dict;

    SYNBL_DICT_V* synbl_cur;

    vector<string> KT;  // 关键字表
    vector<string> PT;  // 界符表
    vector<Charac> cT;  // 字符常量表
    vector<string> ST;  // 字符串常量表
    vector<Number*> CT; // 数值常量表
    Tables();
    ~Tables();

    void new_synbl(string name);

    SYNBL_V* search(SYNBL* table, string src);

    void add(SYNBL* table, string& src);
};

ostream& operator<<(ostream& out, Tables::Number n);
bool operator==(Tables::Number n_1, Tables::Number n_2);

#endif //COMPILE_TABLES_H
