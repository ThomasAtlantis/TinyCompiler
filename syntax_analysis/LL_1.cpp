//
// Created by MAC on 2018/12/17.
//

#include "LL_1.h"
#include "../lexical_analysis/scanner.h"
#include "../data_structure/token.h"

// LL1构造函数，初始化成员变量
LL1::LL1(Grammar& grammar, Scanner& sc): G(grammar), scanner(sc) {
    // 注意在此程序中必须保证“#”在第一位
    available = false;
    if (is_available()) { // 检验文法的合法性
        available = true;
        // 计算栈顶符号列表stack_tops和当前符号列表currents
        currents.emplace_back(Grammar::bound);
        stack_tops.emplace_back(Grammar::bound);
        for (auto iter_1: Grammar::G) {
            stack_tops.push_back(iter_1.first); // 将非终结符加入栈顶符列表
            for (auto iter_2: iter_1.second) {
                for (auto iter_3: iter_2) {
                    if (grammar.symbol_type(iter_3) == -1) {
                        // 将终结符加入当前符列表中，注意查重
                        if (!count(currents.begin(), currents.end(), iter_3))
                            currents.push_back(iter_3);
                        // 将非首位终结符加入栈顶符列表中，注意查重
                        if (iter_3 != iter_2.front() && !count(stack_tops.begin(), stack_tops.end(), iter_3))
                            stack_tops.push_back(iter_3);
                    }
                }
            }
        }
        // 为LL1分析表开辟内存空间
        table = new Analyze_table_item* [stack_tops.size()];
        for (unsigned int i = 0; i < stack_tops.size(); i ++) {
            table[i] = new Analyze_table_item [currents.size()];
        }
        initialize_table();
    }
}
LL1::~LL1() = default;

bool LL1::is_available() {
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

// 获取LL1分析表某项的指针，按行列表头：栈顶符和当前符查找
LL1::Analyze_table_item* LL1::get_op(string stack_top, string current) {
    int m = -1, n = -1;
    for (unsigned int i = 0; i < stack_tops.size(); i ++) {
        if (stack_tops[i] == stack_top) {
            m = i;
            break;
        }
    }
    for (unsigned int i = 0; i < currents.size(); i ++) {
        if (currents[i] == current) {
            n = i;
            break;
        }
    }
    // 将行列表头关键字转换为数组下标
    if (m != -1 && n != -1) return &table[m][n];
    else return nullptr; // 如果没找到该元素则返回空
}

// 设置LL1表中某项的值，stack_op read_op 分别是对栈的操作和对输入流的操作
bool LL1::set_op(string stack_top, string current, vector<string> stack_op, char read_op) {
    Analyze_table_item* p = get_op(std::move(stack_top), std::move(current));
    if (!p) return false;
    p->stack_op = std::move(stack_op);
    p->read_op = read_op;
    return true;
}

// 输出LL1表以供调试
void LL1::print_table() {
    cout << setw(14) << " ";
    for (const auto &current : currents) {
        cout << setw(14) << current;
    }
    cout << endl;
    for (unsigned int i = 0; i < stack_tops.size(); i ++) {
        cout << setw(14) << stack_tops[i];
        for (unsigned int j = 0; j < currents.size(); j ++) {
            if (!table[i][j].stack_op.empty()) {
                cout << setw(10) << join(table[i][j].stack_op);
                cout << ", " << setw(2) << table[i][j].read_op;
            } else cout << setw(14) << " ";
        }
        cout << endl;
    }
}

// 初始化LL1分析表
void LL1::initialize_table() {
    set_op(Grammar::bound, Grammar::bound, vector<string>{"OK"}, '\0');
    for (auto iter = stack_tops.begin() + 1; iter != stack_tops.end(); iter++) {
        if (G.symbol_type(*iter) != 1) {
            if ((*iter).find("qua") == 0) { // 如果是翻译文法对应符号
                for (const auto &cur_it: currents) { // 将翻译文法对应符号的一行全填为（epsilon，P）
                    set_op(*iter, cur_it, vector<string>{Grammar::null}, 'P');
                }
            } else { // 其他终结符的处理
                set_op(*iter, *iter, vector<string>{Grammar::null}, 'N');
            }
        } else { // 如果当前遍历到的符号为非终结符
            auto p = Grammar::G.find(*iter);
            for (auto &it: p->second) {
                switch (G.symbol_type(it[0])) { // 判断右部每个产生式的开头符号
                    case Grammar::Null: // epsilon
                    {
                        set<string> select = G.select_set_of(*iter, it);
                        for (const auto &set_it: select) {
                            set_op(*iter, set_it, vector<string>{Grammar::null}, 'P');
                        }
                        break;
                    }
                    case Grammar::VN: // VN
                    {
                        set<string> select = G.select_set_of(*iter, it);
                        Grammar::Right_symbols rs = it;
                        reverse(rs.begin(), rs.end());
                        for (const auto &set_it : select) {
                            set_op(*iter, set_it, rs, 'P');
                        }
                        break;
                    }
                    case Grammar::VT: // VT
                    {
                        Grammar::Right_symbols rs;
                        if (it.size() > 1) {
                            rs.insert(rs.begin(), it.begin() + 1, it.end());
                            reverse(rs.begin(), rs.end());
                        } else {
                            rs.push_back(Grammar::null);
                        }
                        set_op(*iter, it[0], rs, 'N');
                    }
                }
            }
        }
    }
}

// 将当前Token转为符合文法的字符串
string LL1::token2str(Token token) {
    if (token.kind == "K" || token.kind == "P") {
        return token.src;
    } else if (token.kind == "I") {
        return "@I";
    } else if (token.kind == "C") {
        switch (G.tables.CT[token.index]->type) {
        case Tables::INTEGER: return "@INT";
        case Tables::FLOAT  : return "@FLT";
        case Tables::BOOLEAN: return token.src;
        }
    } else if (token.kind == "c") {
        return "@CH";
    } else if (token.kind == "S") {
        return "@STR";
    } else if (token.kind == Grammar::bound) {
        return Grammar::bound;
    } else return "";
}

// 语法分析主控函数
vector<Quarternary> LL1::check_trans() {
    vector<Quarternary> Qs; // 返回四元式序列
    vector<Tables::Number*> operands; // 操作数栈，存四元式中的指针
    vector<string> syn {Grammar::bound, Grammar::S}; // 分析栈
    Token token {};
    try {
        token = scanner.scan_next();
    } catch (ScannerException& e) {
        if (e.get_log() != Errors::fake_error[0]) throw e;
    }
    while (true) {
        string w = token2str(token);
        Analyze_table_item* p = get_op(syn.back(), w); // 查LL1分析表
        if (!p || (p->stack_op).empty()) { // 如果查表越界或查到的表项为空则报错
            throw SyntaxException(scanner.get_line(), Errors::syntax_error[3] + ": " + token.src);
        } else if (p->stack_op[0] == "OK") { // 如果查到OK则接收字符串返回四元式序列
            return Qs;
        } else {
            syn.pop_back();
            if (p->stack_op[0] != Grammar::null) // 如果栈操作不为弹栈，则将栈操作压栈
                syn.insert(syn.end(), (p->stack_op).begin(), (p->stack_op).end());
            if (syn.back().length() >= 4 && syn.back().find("qua") == 0) { // 如果栈顶符为翻译文法符号
                string operat = (syn.back()).substr(3, syn.back().length() - 3);
/************** 语义动作 **************/

                // 对于quap将操作数保存入栈
                if (operat == "p") {
                    operands.push_back(G.tables.CT[token.index]);
                }

                // 对于qua. 处理符号运算
                else if (operat == ".") {
                    auto * res_1 = new Tables::Number;
                    auto * res_2 = operands.back();
                    auto * res = new Tables::Number;
                    res_1->type = Tables::INTEGER;
                    res_1->value.i = 0;
                    operands.pop_back();
                    Quarternary Q = {"-", res_1, res_2, res};
                    Qs.push_back(Q);
                    operands.push_back(res);
                }

                // main后左花括号新建符号表
                else if (operat == "_new_synbl") {
                    G.tables.new_synbl("main");
                }

                // 对于其他二元运算
                else if (operat == "+" or operat == "-" or operat == "*" or operat == "/") {
                    auto * res_2 = operands.back();
                    operands.pop_back();
                    auto * res_1 = operands.back();
                    operands.pop_back();
                    auto * res = new Tables::Number;
                    Quarternary Q = {operat, res_1, res_2, res};
                    Qs.push_back(Q);
                    operands.push_back(res);
                }
/************** 语义动作 **************/
            }
            if (p->read_op == 'N') { // 如果当前输入流操作为N，则读下一Token
                try {
                    token = scanner.scan_next();
                } catch (ScannerException& e) {
                    if (e.get_log() == Errors::fake_error[0]) {
                        if (w != Grammar::bound) token = {Grammar::bound, 0, ""};
                        else throw SyntaxException(scanner.get_line(), Errors::syntax_error[3]);
                    } else throw e;
                }
            }
        }
    }
}