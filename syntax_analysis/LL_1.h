//
// Created by MAC on 2018/12/17.
//

#ifndef COMPILE_LL_1_H
#define COMPILE_LL_1_H

#include "../utility/utility.h"
#include "../data_structure/data_structure.h"
#include "../lexical_analysis/scanner.h"

class LL1 {
private:
    string token2str(Token token);
public:
    typedef struct {
        vector<string> stack_op; // 对分析栈的操作，文法符号的向量，将来压入栈中
        char read_op;            // 对输入流的控制，'N'表示读下一个token，'P'表示不读
    } Analyze_table_item;        // 分析表中的每一项

    Grammar& G; // 对文法的引用
    Scanner& scanner;
    vector<string> currents;     //当前符集合
    vector<string> stack_tops;   //栈顶符集合
    Analyze_table_item** table;  //分析表
    Tables::TVAL** type_trans ; // 隐式类型转换表
    bool available; //文法是否可用，即文法是否符合LL(1)文法
    LL1(Grammar& grammar, Scanner& scanner);

    ~LL1();
    bool is_available(); // 判断文法是否符合LL1文法
    Analyze_table_item* get_op(string stack_top, string current); // 获取LL1分析表的某一项的指针
    bool set_op(string stack_top, string current, vector<string> stack_op, char read_op); // 设置LL1分析表的某一项的值
    void print_table(); // 输出LL1分析表，用于测试
    void initialize_table(); //初始化LL1分析表
    void initialize_type_trans();//初始化隐式类型转换表
    vector<Quarternary> check_trans(); // 语法分析函数
    Token type_check_tool(
            vector<Quarternary>& Qs,
            const Token& src, const Token& dst,
            const string& op
    );
};

#endif //COMPILE_LL_1_H
