//
// Created by MAC on 2018/12/17.
//

#include "LL_1.h"
#include "../lexical_analysis/scanner.h"
#include "../data_structure/token.h"
#include "../data_structure/tables.h"

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
                        vector<string> stack_op;
                        if (it.size() > 1) {
                            for (const auto &symbol: it)
                                if (symbol.find("qua") != string::npos)
                                    stack_op.insert(stack_op.begin(), symbol);
                        } else stack_op.push_back(Grammar::null);
                        for (const auto &set_it: select) {
                            set_op(*iter, set_it, stack_op, 'P');
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
    if (token->cate == Tables::KEYWORD || token->cate == Tables::DELIMITER) {
        return token->src;
    } else if (token->cate == Tables::VARIABLE || token->cate == Tables::TYPE) {
        return "@I";
    } else if (token->cate == Tables::CONSTANT) {
        switch (token->type->tval) {
        case Tables::INTEGER: return "@INT";
        case Tables::FLOAT  : return "@FLT";
        case Tables::CHAR   : return "@CH";
        case Tables::STRING : return "@STR";
        case Tables::BOOLEAN: return token->src;
        default: return "";
        }
    } else if (token->cate == Tables::BOUND) {
        return Grammar::bound;
    } else return "";
}

// 语法分析主控函数
vector<Quarternary> LL1::check_trans() {
    vector<Quarternary> Qs; // 返回四元式序列
    vector<string> syn {Grammar::bound, Grammar::S}; // 分析栈

    /************** 语义动作用到的变量 **************/
    vector<Tables::SYNBL_V*> operands; // 操作数栈，存四元式中的指针
    string function_name;

    // For declaration
    Tables::TVAL declare_type = Tables::INTEGER;
    Tables::SYNBL_V* declare_id = nullptr;
    size_s array_len = 0;

    // For struct
    Tables::SYNBL_V* struct_id = nullptr;

    // For for_loop adjustment
    size_t for_invert_begin = 0;
    size_t for_invert_end = 0;
    /************** 语义动作用到的变量 **************/

    Token token = nullptr;
    try {
        token = scanner.scan_next();
    } catch (ScannerException& e) {
        if (e.get_log() != Errors::fake_error[0]) throw e;
    }
    while (true) {
        string w = token2str(token);
        Analyze_table_item* p = get_op(syn.back(), w); // 查LL1分析表
        if (!p || (p->stack_op).empty()) { // 如果查表越界或查到的表项为空则报错
            cout << syn.back() << ", " << w << endl;
            throw SyntaxException(scanner.get_line(), Errors::syntax_error[3] + ": " + token->src);
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
                    operands.push_back(token);
                } else if (operat == "p_id") {
                    operands.push_back(declare_id);
                } else if (operat == ".") { // 对于qua. 处理符号运算
                    auto * num = new Tables::Number;
                    num->type = Tables::INTEGER;
                    num->value.i = 0;
                    auto * src_1 = new Tables::SYNBL_V {
                        "0", G.tables.synbl_cur->get_xtp('i'), Tables::CONSTANT, num
                    };
                    auto * src_2 = operands.back();
                    auto * dst = G.tables.synbl_cur->add(Tables::get_global_name());
                    operands.pop_back();
                    Quarternary Q = {"-", src_1, src_2, dst};
                    Qs.push_back(Q);
                    operands.push_back(dst);
                    // 对于其他二元运算
                } else if (operat == "+" or operat == "-" or operat == "*" or operat == "/") {
                    auto * src_2 = operands.back();
                    operands.pop_back();
                    auto * src_1 = operands.back();
                    operands.pop_back();
                    auto * dst = G.tables.synbl_cur->add(Tables::get_global_name());
                    Quarternary Q = {operat, src_1, src_2, dst};
                    Qs.push_back(Q);
                    operands.push_back(dst);
                } else if (operat == "=") {
                    auto * src = operands.back();
                    operands.pop_back();
                    auto * dst = operands.back();
                    operands.pop_back();
                    Quarternary Q {operat, src, nullptr, dst};
                    Qs.push_back(Q);
                } else if (operat == "[]") {
                    auto * index = operands.back();
                    operands.pop_back();
                    auto * array = operands.back();
                    operands.pop_back();
                    auto * dst = G.tables.synbl_cur->add(Tables::get_global_name());
                    Quarternary Q {operat, array, index, dst};
                    Qs.push_back(Q);
                    operands.push_back(dst);
                }

                else if (operat == ">" or operat == "<" or operat == "<=" or
                    operat == ">=" or operat == "==" or operat == "!=" ) {
                    auto * src_2 = operands.back();
                    operands.pop_back();
                    auto * src_1 = operands.back();
                    operands.pop_back();
                    auto * dst = G.tables.synbl_cur->add(Tables::get_global_name());
                    Quarternary Q = {operat, src_1, src_2, dst};
                    Qs.push_back(Q);
                    operands.push_back(dst);
                }

                // 左花括号后新建符号表
                else if (operat == "_new_synbl") {
                    G.tables.new_synbl(function_name);
                }
                else if (operat == "_return") {
                    G.tables.ret_synbl();
                }

                // 储存当前函数名
                else if (operat == "_sav_func_name") {
                    if (G.tables.search_func(token->src) != nullptr)
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[7] + ": " + token->src);
                    function_name = token->src;
                }
                else if (operat == "_gen_func_name") {
                    function_name = token->src + Tables::get_global_name();
                }

                // 获取类型
                else if (operat == "_int") {
                    declare_type = Tables::INTEGER;
                } else if (operat == "_float") {
                    declare_type = Tables::FLOAT;
                } else if (operat == "_bool") {
                    declare_type = Tables::BOOLEAN;
                } else if (operat == "_char") {
                    declare_type = Tables::CHAR;
                } else if (operat == "_declare_id") {
                    if (token->type != nullptr)
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[5] + ": " + token->src);
                    declare_id = token;
                } else if (operat == "_declare_arr") {
                    auto * num = (Tables::Number*)token->addr;
                    if (num->value.i <= 1)
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[4] + ": " + token->src);
                    array_len = (size_s)num->value.i;
                } else if (operat == "_declare") {
                    SYNBL* synbl = G.tables.synbl_cur;
                    if (array_len != 0) {
                        // 新建数组信息
                        auto * array_info = new Tables::AINEL;
                        array_info->low = 0;
                        array_info->up = array_len - 1;
                        array_info->ctp = synbl->get_xtp(declare_type);
                        array_info->clen = Tables::get_size_of(declare_type);
                        synbl->ainel.push_back(array_info);

                        // 注册数组信息入类型表
                        auto * typel = new Tables::TYPEL;
                        typel->tval = Tables::ARRAY;
                        typel->tptr = array_info;
                        synbl->typel.push_back(typel);

                        // 填长度表
                        auto * len = new Tables::LENL {array_len * array_info->clen};
                        synbl->lenl.push_back(len);

                        // 填主符号表
                        // 填匿名类型
                        Tables::SYNBL_V* type = synbl->add(Tables::get_global_name());
                        type->cate = Tables::TYPE;
                        type->type = typel;
                        type->addr = len;
                        // 填声明变量
                        declare_id->cate = Tables::VARIABLE;
                        declare_id->type = typel;
                        declare_id->addr = nullptr;
                        // TODO: 计算addr，即variable的区距，有赖于活动记录栈

                        // 更新值
                        array_len = 0;
                    } else {
                        declare_id->cate = Tables::VARIABLE;
                        declare_id->type = synbl->get_xtp(declare_type);
                        declare_id->addr = nullptr;
                        // TODO: 计算addr，即variable的区距，有赖于活动记录栈
                    }
                }

                else if (operat == "_check_def") { // 暂时没用到
                    if (token->type == nullptr and G.tables.search(token->src) == nullptr)
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[6] + ": " + token->src);
                } else if (operat == "_check_def_e") {
                    Token tmp = operands.back();
                    if (tmp->type == nullptr and G.tables.search(tmp->src) == nullptr)
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[6] + ": " + tmp->src);
                }

                else if (operat == "_new_synbl_struct") {
                    G.tables.new_synbl(function_name);
                } else if (operat == "_declare_struct_id") {
                    if (token->type != nullptr and token->type->tval != Tables::STRUCTURE)
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[8] + ": " + token->src);
                    struct_id = token;
                } else if (operat == "_struct_def") {
                    SYNBL* synbl = G.tables.synbl_cur;

                    auto * rinfl = new Tables::RINEL;

                    auto * len = new Tables::LENL;
                    synbl->lenl.push_back(len);

                    auto * typel = new Tables::TYPEL;
                    typel->tval = Tables::STRUCTURE;
                    typel->tptr = rinfl;
                    synbl->typel.push_back(typel);

                    struct_id->cate = Tables::TYPE;
                    struct_id->type = typel;
                    struct_id->addr = len;
                } else if (operat == "_struct_check_def") {
                    auto * result = G.tables.search(struct_id->src);
                    if ((struct_id->type == nullptr and (result == nullptr or result->type->tval != Tables::STRUCTURE))
                        or (struct_id->type->tval != Tables::STRUCTURE)) {
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[8] + ": " + struct_id->src);
                    }
                    if (token->type != nullptr)
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[5] + ": " + token->src);
                }

                else if (operat == "if") {
                    auto * src_1 = operands.back();
                    operands.pop_back();
                    Quarternary Q = {operat, src_1, nullptr, nullptr};
                    Qs.push_back(Q);
                } else if (operat == "else") {
                    Quarternary Q = {operat, nullptr, nullptr, nullptr};
                    Qs.push_back(Q);
                } else if (operat == "_endif") {
                    Quarternary Q = {"endif", nullptr, nullptr, nullptr};
                    Qs.push_back(Q);
                } else if (operat == "_endifall") {
                    Quarternary Q = {"endifall", nullptr, nullptr, nullptr};
                    Qs.push_back(Q);
                }

                else if (operat == "wh") {
                    Quarternary Q = {operat, nullptr, nullptr, nullptr};
                    Qs.push_back(Q);
                } else if (operat == "wdo" ) {
                    auto * src_1 = operands.back();
                    operands.pop_back();
                    Quarternary Q = {operat, src_1, nullptr, nullptr};
                    Qs.push_back(Q);
                } else if (operat == "we") {
                    Quarternary Q1 = {"wnxt", nullptr, nullptr, nullptr};
                    Qs.push_back(Q1);
                    Quarternary Q2 = {"wend", nullptr, nullptr, nullptr};
                    Qs.push_back(Q2);
                }

                else if (operat == "for") {
                    Quarternary Q = {operat, nullptr, nullptr, nullptr};
                    Qs.push_back(Q);
                } else if (operat == "fdo") {
                    auto * src_1 = operands.back();
                    operands.pop_back();
                    Quarternary Q = {operat, src_1, nullptr, nullptr};
                    Qs.push_back(Q);
                    for_invert_begin = Qs.size();
                } else if (operat == "fsav") {
                    for_invert_end = Qs.size() - 1;
                } else if (operat == "fe") {
                    if (!(for_invert_begin == 0 and for_invert_end == 0)) {
                        for (size_t i = for_invert_begin; i <= for_invert_end; i++) {
                            auto tmp = Qs[for_invert_begin];
                            Qs.erase(Qs.begin() + for_invert_begin);
                            Qs.push_back(tmp);
                        }
                    }
                    Quarternary Q1 = {"fnxt", nullptr, nullptr, nullptr};
                    Qs.push_back(Q1);
                    Quarternary Q2 = {"fend", nullptr, nullptr, nullptr};
                    Qs.push_back(Q2);
                }
            /************** 语义动作 **************/
            }
            if (p->read_op == 'N') { // 如果当前输入流操作为N，则读下一Token
                try {
                    token = scanner.scan_next();
                } catch (ScannerException& e) {
                    if (e.get_log() == Errors::fake_error[0]) {
                        if (w != Grammar::bound)
                            token = new Tables::SYNBL_V {Grammar::bound, nullptr, Tables::BOUND, nullptr};
                        else throw SyntaxException(scanner.get_line(), Errors::syntax_error[3]);
                    } else throw e;
                }
            }
        }
    }
}