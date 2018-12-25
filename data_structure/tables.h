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
        size_t clen; // ����ÿ��Ԫ����ռ���ֽ���
        struct typel* ctp;
    } AINEL; // ������Ϣ��
    typedef struct rinfl_v {
        string id;
        size_t off;
        struct typel* tp;
    } RINFL_V; // �ṹ����Ϣ�����
    typedef vector<RINFL_V*> RINFL;
    typedef struct typel {
        TVAL tval;
        void* tptr; // AINEL* or RINFL*
    } TYPEL; // ���ͱ�
    typedef struct {
        string src;
        struct typel* type;
        CATE cate;
        void* addr;
    } SYNBL_V; // ���ű�����ֵ
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
    } PARAM_V; // �����βα�
    typedef vector<PARAM_V*> PARAM;
    typedef struct pfinfl {
        size_s level;
        size_t off;
        size_s fn = 0;
        size_t entry;
        PARAM* param;
    } PFINFL; // ������Ϣ��

    map<string, SYNBL*> synbl_dict; // ���ű�����ֵ䣬һ��������Ӧһ�ű�
    SYNBL* synbl_cur; // ά����ǰ�ķ��ű�
    static size_t global_count; // ȫ��Ψһ��ʶ������
    static size_t heap_top;
    vector<string> KT;  // �ؼ��ֱ�
    vector<string> PT;  // �����
    vector<Charac*> cT; // �ַ�������
    vector<Charac*> ST; // �ַ���������
    vector<Number*> CT; // ��ֵ������
    Tables();
    ~Tables();

    // �½�һ�ŷ��ű�
    void new_synbl(string name);

    void ret_synbl();

    // ��ȡһ��ȫ��Ψһ��ʶ�����֣�tx
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

    vector<Tables::SYNBL_V*> content; // ���ű������Ե���Դ��Ϊ�������ֵ�
    vector<Tables::TYPEL*> typel;   // ���ͱ�
//    vector<Tables::RINEL*> rinel;   // �ṹ����Ϣ��
    vector<Tables::AINEL*> ainel;   // ������Ϣ��
//    vector<Tables::LENL *> lenl;     // ���ȱ�
 //   vector<Tables::PARAM*> param;   // �β���Ϣ��
    Tables::PFINFL pfinfl;//������Ϣ��

    SYNBL();
    ~SYNBL();

    // ��������������û������ʶ����Ҳ��������ʱ����
    Tables::SYNBL_V* add(string src);

    // �ڱ��в��ұ�ʶ��src�Ƿ����
    Tables::SYNBL_V* search(const string& src);

    // ��ȡitp, rtp, ctp, btp��stp��ָ��
    Tables::TYPEL* get_xtp(char kind);
    Tables::TYPEL* get_xtp(Tables::TVAL kind);

    void show_tree(vector<bool>& vec);
};
// ���������
ostream& operator<<(ostream& out, Tables::Number n);
bool operator==(Tables::Number n_1, Tables::Number n_2);

// �������λ���±�
int find(vector<Charac*> vec, string key);
int find(vector<Charac*> vec, Charac key);
int find(vector<Tables::Number*> nums, Tables::Number key);

#endif //COMPILE_TABLES_H
