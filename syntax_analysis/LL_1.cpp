//
// Created by MAC on 2018/12/17.
//

#include "LL_1.h"
#include "../lexical_analysis/scanner.h"
#include "../data_structure/token.h"
#include "../data_structure/tables.h"
#include "../data_structure/quarternary.h"

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
        //为隐式类型转换表开辟空间
        type_trans = new Tables::TVAL* [7];
        for(size_s i = 0; i < 7; i++){
            type_trans[i] = new Tables::TVAL [7];
        }
        initialize_type_trans();
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
    } else if (token->cate == Tables::VARIABLE || token->cate == Tables::TYPE || token->cate == Tables::DOMAINN ||
               token->cate == Tables::VARIABLE_VALUE || token->cate == Tables::VARIABLE_ADDRESS) {
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

Token LL1::type_check_tool(vector<Quarternary>& Qs, const Token& src, const Token& dst, const string& op) {
    auto result = type_trans[src->type->tval][dst->type->tval];
    if(result == Tables::NONE) {
        throw SyntaxException(
                scanner.get_line(),
                Errors::syntax_error[14] + ": " + Tables::get_type_name(src->type->tval) +
                " and " + Tables::get_type_name(dst->type->tval)
        );
    } else {
        auto *after_change = G.tables.synbl_cur->add(Tables::get_global_name());
        after_change->type = new Tables::TYPEL;
        after_change->type->tptr = nullptr;
        char type[] = {'f', 'i', 'c', 'b', 's', 't', 'a', 'n'};
        string cast = "cast_";
        Quarternary Q;
        if (op == "dst") {
            after_change->type->tval = dst->type->tval;
            cast = cast + type[src->type->tval] + "2" + type[after_change->type->tval];
            Q = {cast, src, nullptr, after_change};
        } else {
            after_change->type->tval = result;
            Token p = nullptr;
            if (result != src->type->tval) p = src;
            else p = dst;
            cast = cast + type[p->type->tval] + "2" + type[after_change->type->tval];
            Q = {cast, p, nullptr, after_change};
        }
        Qs.push_back(Q);
        if (Q.a->type->tval < Q.res->type->tval) {
            cout << "(" + to_string(scanner.get_line()) +") "
                 << Errors::warnings[0]
                 << ": from " + Tables::get_type_name(src->type->tval)
                 << " to " + Tables::get_type_name(dst->type->tval)
                 << endl;
        }
        if (Q.a->type->tval == Tables::BOOLEAN and Q.res->type->tval == Tables::INTEGER) {
            //Qs.pop_back();
            auto * one_num = new Tables::Number;
            one_num->type = Tables::INTEGER;
            one_num->value.i = 1;
            auto * one = new Tables::SYNBL_V {
                "1", G.tables.synbl_cur->get_xtp('i'), Tables::CONSTANT, one_num
            };
            auto * zero_num = new Tables::Number;
            zero_num->type = Tables::INTEGER;
            zero_num->value.i = 0;
            auto * zero = new Tables::SYNBL_V {
                "0", G.tables.synbl_cur->get_xtp('i'), Tables::CONSTANT, zero_num
            };
            Quarternary q1 {"if", Q.a, nullptr, nullptr};
            Quarternary q2 {"=", one, nullptr, after_change};
            Quarternary q3 {"endif", nullptr, nullptr, nullptr};
            Quarternary q4 {"=", zero, nullptr, after_change};
            Quarternary q5 {"endifall", nullptr, nullptr, nullptr};
            Qs.pop_back();
            Qs.push_back(q1);
            Qs.push_back(q2);
            Qs.push_back(q3);
            Qs.push_back(q4);
            Qs.push_back(q5);
        }
        return after_change;
    }
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
    Tables::SYNBL_V* struct_variable = nullptr;

    // For for_loop adjustment
    size_t for_invert_begin = 0;
    size_t for_invert_end = 0;

    // For function def and call
    bool is_addrParm = false;
    int curParam=0;
    string call_func_name;
    /************** 语义动作用到的变量 **************/

    Token token = nullptr;
    Token token_pre = nullptr; // 储存上一个token，方便报错
    size_s line_pre = 0;
    try {
        token = scanner.scan_next();
        token_pre = token;
        line_pre = scanner.get_line();
    } catch (ScannerException& e) {
        if (e.get_log() != Errors::fake_error[0]) throw e;
    }
    while (true) {
        string w = token2str(token);
        Analyze_table_item* p = get_op(syn.back(), w); // 查LL1分析表
        if (!p || (p->stack_op).empty()) { // 如果查表越界或查到的表项为空则报错
            // cout << syn.back() << ", " << w << endl;
            throw SyntaxException(line_pre, Errors::syntax_error[3] + ": " + token_pre->src);
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
                } if (operat == "d") {
                    operands.pop_back();
                } else if (operat == "p_id") {
                    operands.push_back(declare_id);
                } else if (operat == ".") { // 对于qua. 处理符号运算
                    auto * src_2 = operands.back();
                    if (src_2->type->tval != Tables::INTEGER and src_2->type->tval != Tables::FLOAT) {
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[14] + ": " + token->src);
                    }

                    auto * num = new Tables::Number;
                    num->type = Tables::INTEGER;
                    num->value.i = 0;
                    auto * src_1 = new Tables::SYNBL_V {
                        "0", G.tables.synbl_cur->get_xtp('i'), Tables::CONSTANT, num
                    };
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
                    dst->type = new Tables::TYPEL;
                    if (src_1->type->tval != src_2->type->tval) {
                        Token after_change = type_check_tool(Qs, src_1, src_2, "res");
                        Quarternary Q = {operat, src_1, src_2, dst};
                        if (after_change->type->tval != src_1->type->tval) Q.a = after_change;
                        else Q.b = after_change;
                        dst->type->tval = after_change->type->tval;
                        Qs.push_back(Q);
                        operands.push_back(dst);
                    } else {
                        dst->type->tval = src_1->type->tval;
                        Quarternary Q = {operat, src_1, src_2, dst};
                        Qs.push_back(Q);
                        operands.push_back(dst);
                    }
                } else if (operat == "=") {
                    auto * src = operands.back();
                    operands.pop_back();
                    auto * dst = operands.back();
                    operands.pop_back();
                    if (src->type->tval != dst->type->tval) {
                            Token after_change = type_check_tool(Qs, src, dst, "dst");
                            Quarternary Q1 = {operat, after_change, nullptr, dst};
                            Qs.push_back(Q1);
                    } else {
                        Quarternary Q {operat, src, nullptr, dst};
                        Qs.push_back(Q);
                    }
                } else if (operat == "[]") {
                    auto * index = operands.back();
                    operands.pop_back();
                    auto * array = operands.back();
                    operands.pop_back();

                    if (index->cate == Tables::CONSTANT) {
                        auto * num = (Tables::Number *) index->addr;
                        auto * ainel = (Tables::AINEL *) array->type->tptr;
                        if (num->value.i > ainel->up or num->value.i < ainel->low)
                            throw SyntaxException(line_pre, Errors::syntax_error[12] + ": " + token_pre->src);
                    } else if (index->cate == Tables::VARIABLE) {
                        if (index->type->tval != Tables::INTEGER)
                            throw SyntaxException(line_pre, Errors::syntax_error[11] + ": " + token_pre->src);
                    } else {
                        throw SyntaxException(line_pre, Errors::syntax_error[11] + ": " + token_pre->src);
                    }

                    auto * dst = G.tables.synbl_cur->add(Tables::get_global_name());
                    dst->type = new Tables::TYPEL;
                    auto * ainel = (Tables::AINEL *) array->type->tptr;
                    dst->type->tval = ainel->ctp->tval;
                    dst->type->tptr = nullptr;
                    Quarternary Q {operat, array, index, dst};
                    Qs.push_back(Q);
                    operands.push_back(dst);
                } else if (operat == "_member") {
                    auto * member = operands.back();
                    operands.pop_back();
                    auto * master = operands.back();
                    operands.pop_back();

                    if (master->cate != Tables::VARIABLE or master->type->tval != Tables::STRUCTURE)
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[13] + ": " + master->src);

                    bool has_domain = false;
                    Tables::TVAL tval;
                    const auto& rinfl = (Tables::RINFL *)master->type->tptr;
                    for (const auto& it: *rinfl) {
                        if (it->id == member->src) {
                            has_domain = true;
                            tval = it->tp->tval;
                            //cout << it->off << endl;
                        }
                    }
                    if (!has_domain) {
                        throw SyntaxException(line_pre, Errors::syntax_error[9] + ": " + token_pre->src);
                    }
                    auto * dst = G.tables.synbl_cur->add(Tables::get_global_name());
                    dst->type = new Tables::TYPEL;
                    dst->type->tptr = nullptr;
                    dst->type->tval = tval;
                    Quarternary Q {".", master, member, dst};
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
                    dst->type = new Tables::TYPEL {Tables::BOOLEAN, nullptr};
                    if (src_1->type->tval != src_2->type->tval) {
                        Token after_change = type_check_tool(Qs, src_1, src_2, "res");
                        Quarternary Q = {operat, src_1, src_2, dst};
                        if (after_change->type->tval != src_1->type->tval) Q.a = after_change;
                        else Q.b = after_change;
                        Qs.push_back(Q);
                        operands.push_back(dst);
                    } else {
                        Quarternary Q = {operat, src_1, src_2, dst};
                        Qs.push_back(Q);
                        operands.push_back(dst);
                    }
                }

                // 输入输出语句的翻译
                else if (operat == "_putc") {
                    operands.push_back(token);
                    Quarternary Q {"putc", token, nullptr, nullptr};
                    Qs.push_back(Q);
                } else if (operat == "_puts") {
                    Quarternary Q {"puts", token, nullptr, nullptr};
                    Qs.push_back(Q);
                } else if (operat == "_getc") {
                    operands.push_back(token);
                    Quarternary Q {"getc", token, nullptr, nullptr};
                    Qs.push_back(Q);
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
                    if (token->type != nullptr) {
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[5] + ": " + token->src);
                    } else if (G.tables.search_func(token->src) != nullptr) {
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[7] + ": " + token->src);
                    }
                    function_name = token->src;
                    token->cate = Tables::FUNCTION;
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
                    Tables::SYNBL_V* type;
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

                        // 填主符号表
                        // 填匿名类型
                        type = synbl->add(Tables::get_global_name());
                        type->cate = Tables::TYPE;
                        type->type = typel;
                        type->addr = len;
                        // 填声明变量
                        declare_id->cate = Tables::VARIABLE;
                        declare_id->type = typel;
                        declare_id->addr = nullptr;

                        // 更新值
                        array_len = 0;
                    } else {
                        declare_id->type = synbl->get_xtp(declare_type);
                        declare_id->addr = nullptr;
                        declare_id->cate = Tables::VARIABLE;
                    }
                    declare_id->addr = new Tables::ADDR {synbl, synbl->vall_top};
                    if (is_addrParm) {
                        synbl->vall_top += 2;
                    } else if ( declare_id->type->tval == Tables::ARRAY ){
                        auto * len = (Tables::LENL*)type->addr;
                        synbl->vall_top += *len;
                    } else {
                        synbl->vall_top += G.tables.get_size_of(declare_id->type->tval);
                    }
                    cout << declare_id->src << ": " << ((Tables::ADDR *)declare_id->addr)->off << endl;
                }

                else if (operat == "_check_def") { // 暂时没用到
                    if (token->type == nullptr and G.tables.search(token->src) == nullptr)
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[6] + ": " + token->src);
                } else if (operat == "_check_def_e") {
                    Token tmp = operands.back();
                    if (tmp->type == nullptr) {
                        auto * result = G.tables.search(tmp->src);
                        if (result == nullptr) {
                            throw SyntaxException(scanner.get_line(), Errors::syntax_error[6] + ": " + tmp->src);
                        } else {
                            G.tables.synbl_cur->content.pop_back();
                            G.tables.synbl_cur->content.push_back(result);
                            operands.pop_back();
                            operands.push_back(result);

                        }
                    }
                }

                else if (operat == "_new_synbl_struct") {
                    G.tables.new_synbl(function_name);
                } else if (operat == "_declare_struct_id") {
//                    if (token->type != nullptr and token->type->tval != Tables::STRUCTURE)
//                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[8] + ": " + token->src);
                    struct_id = token;
                } else if (operat == "_struct_def") {
                    SYNBL* synbl = G.tables.synbl_cur;
                    auto * rinfl = new Tables::RINFL;
                    auto * len = new Tables::LENL {0};
                    auto * typel = new Tables::TYPEL;
                    typel->tval = Tables::STRUCTURE;
                    typel->tptr = rinfl;
                    synbl->typel.push_back(typel);
                    struct_id->cate = Tables::TYPE;
                    struct_id->type = typel;
                    struct_id->addr = len;
                } else if (operat == "_struct_check_def") {
                    auto * result = G.tables.search(struct_id->src);
                    if (struct_id->type == nullptr) {
                        if (result == nullptr or result->type->tval != Tables::STRUCTURE)
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[8] + ": " + struct_id->src);
                        else struct_id = result;
                    } else if (struct_id->type->tval != Tables::STRUCTURE) {
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[8] + ": " + struct_id->src);
                    }
                    if (token->type != nullptr)
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[5] + ": " + token->src);
                    struct_variable = token;
                } else if (operat == "_struct_declare") {
                    struct_variable->cate = Tables::VARIABLE;
                    struct_variable->type = struct_id->type;
                    auto * synbl = G.tables.synbl_cur;
                    struct_variable->addr = new Tables::ADDR {synbl, synbl->vall_top};
                    auto * len = (Tables::LENL *) struct_id->addr;
                    synbl->vall_top += *len;
                    cout << struct_variable->src << ": " << ((Tables::ADDR *)struct_variable->addr)->off << endl;
                } else if (operat == "_instruct_declare") {
                    SYNBL* synbl = G.tables.synbl_cur;
                    Tables::SYNBL_V* type = nullptr;
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
                        // 填主符号表
                        // 填匿名类型
                        type = synbl->add(Tables::get_global_name());
                        type->cate = Tables::TYPE;
                        type->type = typel;
                        type->addr = len;
                        // 填声明变量
                        declare_id->cate = Tables::DOMAINN;
                        declare_id->type = typel;
                        declare_id->addr = nullptr;
                        // 更新值
                        array_len = 0;
                    } else {
                        declare_id->cate = Tables::DOMAINN;
                        declare_id->type = synbl->get_xtp(declare_type);
                        declare_id->addr = nullptr;
                    }
                    auto * rinfl_v = new Tables::RINFL_V;
                    rinfl_v->id = declare_id->src;
                    rinfl_v->tp = declare_id->type;
                    auto * len = (Tables::LENL *)struct_id->addr;
                    rinfl_v->off = *len;
                    auto * rinfl = (Tables::RINFL *) struct_id->type->tptr;
                    rinfl->push_back(rinfl_v);
                    if (declare_id->type->tptr == nullptr) {
                        *len += Tables::get_size_of(declare_id->type->tval);
                    } else {
                        auto * len_arr = (Tables::LENL *)type->addr;
                        *len += *len_arr;
                    }
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
                    //循环跳转语句
                else if ( operat == "_continue" ){
                    Quarternary Q = {"continue", nullptr, nullptr, nullptr};
                    Qs.push_back(Q);
                } else if ( operat == "_break" ){
                    Quarternary Q = {"break", nullptr, nullptr, nullptr};
                    Qs.push_back(Q);
                }
                    //跳转语句判定
                else if(operat=="_check_jump"){
                    SYNBL* synbl = G.tables.synbl_cur;
                    bool has_loop = false;
                    while ( synbl != nullptr ) {
                        if(synbl->name.find("while") != string::npos ){
                            has_loop = true;
                            break;
                        }
                        if(synbl->name.find("for") != string::npos ){
                            has_loop = true;
                            break;
                        }
                        synbl = synbl->parent;
                    }
                    if( !has_loop ){
                        throw SyntaxException(line_pre, Errors::syntax_error[10] + ": " + token_pre->src);
                    }
                }

                else if (operat == "_func_return") {
                    Quarternary Q = {"ret", nullptr, nullptr, nullptr};
                    Qs.push_back(Q);
                }

                else if (operat == "_new_pfinfl"){//新建函数信息表
                    SYNBL* synbl = G.tables.synbl_cur;
                    synbl->pfinfl.level = synbl->level;
                    synbl->pfinfl.fn = 0;
                    synbl->pfinfl.off = 3;
                    auto * param_1 = new Tables::PARAM;
                    synbl->pfinfl.param = param_1;
                    auto * temp = new Tables::SYNBL_V;
                    temp->src = function_name;
                    Quarternary Q = {"funcdef", temp , nullptr, nullptr};
                    Qs.push_back(Q);
                } else if (operat == "_declare_param" ){//新建形参
                    SYNBL* synbl = G.tables.synbl_cur;
                    auto * param_v = new Tables::PARAM_V;
                    if(is_addrParm) {
                        declare_id->cate = Tables::VARIABLE_ADDRESS;
                        is_addrParm = false;
                    } else {
                        declare_id->cate = Tables::VARIABLE_VALUE;
                    }
                    param_v->type = declare_id->type;
                    param_v->cate = declare_id->cate;
                    param_v->id = declare_id->src;
                    param_v->off = ((Tables::ADDR *)declare_id->addr)->off;
                    // TODO
//                    if( declare_id->type->tval == Tables::ARRAY ){
//                        auto * len = (Tables::LENL*)type->addr;
//                        synbl->vall_top += *len;
//                    } else {
//                        synbl->vall_top += G.tables.get_size_of(declare_id->type->tval);
//                    }
//                    cout << declare_id->src << ": " << ((Tables::ADDR *)declare_id->addr)->off << endl;

                    synbl->pfinfl.param->push_back(param_v);
                    synbl->pfinfl.fn ++;
//                    Quarternary Q{param_v->id, nullptr , nullptr, nullptr};
//                    Qs.push_back(Q);
                } else if(operat == "_addrParm") {
                    is_addrParm = true;
                }
                else if (operat == "_endproc") {
                    Quarternary Q1 = {"return", nullptr , nullptr, nullptr};
                    Qs.push_back(Q1);
                    Quarternary Q = {"endfdef", nullptr , nullptr, nullptr};
                    Qs.push_back(Q);
                } else if (operat == "_call") {
                    Token tmp = operands.back();
                    Quarternary Q = {"callfunc", tmp , nullptr, nullptr};
                    curParam = 0;
                    Qs.push_back(Q);
                    call_func_name = tmp->src;
                } else if (operat == "_contrast"|| operat == "_contrastC") {
                    Token tmp = operands.back();
                    SYNBL* synbl = G.tables.search_func(call_func_name);
                    if(curParam >= synbl->pfinfl.fn)
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[15]);
                    auto * param = synbl->pfinfl.param;
                    auto * param_value = (*param)[curParam];
                    curParam ++;
                    Quarternary Q;
                    if(operat == "_contrastC") {
                        if(token->type->tval != param_value->type->tval)
                            throw SyntaxException(scanner.get_line(), Errors::syntax_error[16] + ": " + tmp->src);
                        else{
                            tmp = token;
                        }
                    } else {
                        if (tmp->type->tval != param_value->type->tval ) {
                            throw SyntaxException(scanner.get_line(), Errors::syntax_error[16] + ": " + tmp->src);
                        } if(tmp->type->tval == Tables::ARRAY){
                            auto * tmpa = (Tables::AINEL*) tmp->type->tptr;
                            auto * tmpv = (Tables::AINEL*) param_value->type->tptr;
                            if (tmpa->ctp->tval != tmpv->ctp->tval)
                                throw SyntaxException(scanner.get_line(), Errors::syntax_error[17] + ": " + tmp->src);
                            else if (tmpa->up != tmpv->up)
                                throw SyntaxException(scanner.get_line(), Errors::syntax_error[18] + ": " + tmp->src);
                        } else if(tmp->type->tval == Tables::STRUCTURE) {

                            auto * tmpsa = (Tables::RINFL*)tmp->type->tptr;
                            auto * tmpsv = (Tables::RINFL*)param_value->type->tptr;
                            if((*tmpsa).size() != (*tmpsv).size())
                                throw SyntaxException(scanner.get_line(), Errors::syntax_error[19] + ": " + tmp->src);
                            for(int i=0; i<(*tmpsa).size();i++){
                                if((*tmpsa)[i]->id != (*tmpsv)[i]->id)
                                    throw SyntaxException(scanner.get_line(), Errors::syntax_error[19] + ": " + tmp->src);
                            }
                        }
                    }
                    if(param_value->cate == Tables::VARIABLE_ADDRESS or param_value->type->tval == Tables::ARRAY)
                        Q = {"value &", tmp , nullptr, nullptr};
                    else
                        Q = {"value", tmp , nullptr, nullptr};
                    Qs.push_back(Q);
                } else if(operat == "_checknum"){
                    SYNBL* synbl = G.tables.search_func(call_func_name);
                    if(curParam < synbl->pfinfl.fn)
                        throw SyntaxException(scanner.get_line(), Errors::syntax_error[15] );
                }
            /************** 语义动作 **************/
            }
            if (p->read_op == 'N') { // 如果当前输入流操作为N，则读下一Token
                try {
                    token_pre = token;
                    line_pre = scanner.get_line();
                    token = scanner.scan_next();
                } catch (ScannerException& e) {
                    if (e.get_log() == Errors::fake_error[0]) {
                        if (w != Grammar::bound){
                            token_pre = token;
                            line_pre = scanner.get_line();
                            token = new Tables::SYNBL_V{Grammar::bound, nullptr, Tables::BOUND, nullptr};
                        }
                        else throw SyntaxException(scanner.get_line(), Errors::syntax_error[3]);
                    } else throw e;
                }
            }
        }
    }
}

void LL1::initialize_type_trans() {
    for(size_s i = 0; i < 3; i++){
        for(size_s j = 0; j < 3; j++){
            if(i <= j){
                type_trans[i][j] = Tables::TVAL(i);
            }else if(i > j){
                type_trans[i][j] = Tables::TVAL(j);
            }
        }
    }
    for(size_s i = 0; i < 7; i++){
        for(size_s j = 0; j < 7; j++){
            if(i == 3 && j == 3){
                type_trans[i][j] = Tables::INTEGER;
            } else if (i >= 3 || j >= 3) {
                type_trans[i][j] = Tables::NONE;
            }
        }
    }
    type_trans[3][1] = Tables::INTEGER;
//    for (int i = 0; i < 7; i ++) {
//        for (int j = 0; j < 7; j ++) {
//            cout << setw(10) << left << Tables::get_type_name(type_trans[i][j]) << " ";
//        }
//        cout << endl;
//    }
}
