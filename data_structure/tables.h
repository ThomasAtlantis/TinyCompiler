//
// Created by MAC on 2018/12/1.
//

#ifndef COMPILE_TABLES_H
#define COMPILE_TABLES_H

#include "../utility/utility.h"

typedef unsigned int size_s;
typedef char Charac;

class SYNBL;

class Tables {
public:
    struct typel; struct ainel; struct rinfl;
    struct pfinfl; struct VALL; struct param;
    typedef enum {
        FLOAT, INTEGER, CHAR, BOOLEAN, STRING, STRUCTURE, ARRAY, NONE
    } TVAL;
    typedef enum {
        DOMAINN, FUNCTION, CONSTANT, TYPE, VARIABLE,
        VARIABLE_ADDRESS, VARIABLE_VALUE, KEYWORD, DELIMITER, BOUND
    } CATE;
    typedef size_t LENL;
    typedef struct ainel {
        size_t low;
        size_t up;
        size_t clen; // 数组每个元素所占用字节数
        struct typel* ctp;
    } AINEL; // 数组信息表
    typedef struct rinfl_v {
        string id;
        size_t off;
        struct typel* tp;
    } RINFL_V; // 结构体信息表表项
    typedef vector<RINFL_V*> RINFL;
    typedef struct typel {
        TVAL tval;
        void* tptr; // AINEL* or RINFL*
    } TYPEL; // 类型表
    typedef struct {
        string src;
        struct typel* type;
        CATE cate;
        void* addr;
    } SYNBL_V; // 符号表主表值
    typedef struct {
        TVAL type;
        union {
            float f;
            int i;
            bool b;
        } value;
    } Number;
    typedef  struct {
        SYNBL* synbl;
        size_t off;
    } ADDR;
    typedef struct param {
        string id;
        struct typel* type;
        CATE cate;
        size_t off;
    } PARAM_V; // 函数形参表
    typedef vector<PARAM_V*> PARAM;
    typedef struct pfinfl {
        size_s level;
        size_t off;
        size_s fn = 0;
        size_t entry;
        PARAM* param;
    } PFINFL; // 函数信息表

    map<string, SYNBL*> synbl_dict; // 符号表类的字典，一个函数对应一张表
    SYNBL* synbl_cur; // 维护当前的符号表
    static size_t global_count; // 全局唯一标识计数器
    static size_t heap_top;
    vector<string> KT;  // 关键字表
    vector<string> PT;  // 界符表
    vector<Charac*> cT; // 字符常量表
    vector<Charac*> ST; // 字符串常量表
    vector<Number*> CT; // 数值常量表
    Tables();
    ~Tables();

    // 新建一张符号表
    void new_synbl(string name);

    void ret_synbl();

    // 获取一个全局唯一标识的名字：tx
    static string get_global_name();

    static size_s get_size_of(TVAL type);

    static string get_type_name(TVAL type);

    static string get_cate_name(CATE cate);

    SYNBL_V* search(const string &src);

    SYNBL *search_func(const string &src);

};

class SYNBL {
public:

    SYNBL* parent;
    vector<SYNBL*> childs;
    size_t index;
    size_s level;
    string name;
    size_t vall_top;

    vector<Tables::SYNBL_V*> content; // 符号表主表，以单词源码为索引的字典
    vector<Tables::TYPEL*> typel;   // 类型表
//    vector<Tables::RINEL*> rinel;   // 结构体信息表
    vector<Tables::AINEL*> ainel;   // 数组信息表
//    vector<Tables::LENL *> lenl;     // 长度表
 //   vector<Tables::PARAM*> param;   // 形参信息表
    Tables::PFINFL pfinfl;//函数信息表

    SYNBL();
    ~SYNBL();

    // 变量填表，可以是用户定义标识符，也可以是临时变量
    Tables::SYNBL_V* add(string src);

    // 在表中查找标识符src是否存在
    Tables::SYNBL_V* search(const string& src);

    // 获取itp, rtp, ctp, btp和stp的指针
    Tables::TYPEL* get_xtp(char kind);
    Tables::TYPEL* get_xtp(Tables::TVAL kind);

    void show_tree(vector<bool>& vec);
};
// 重载运算符
ostream& operator<<(ostream& out, Tables::Number n);
bool operator==(Tables::Number n_1, Tables::Number n_2);

// 查表，返回位置下标
int find(vector<Charac*> vec, string key);
int find(vector<Charac*> vec, Charac key);
int find(vector<Tables::Number*> nums, Tables::Number key);

#endif //COMPILE_TABLES_H
