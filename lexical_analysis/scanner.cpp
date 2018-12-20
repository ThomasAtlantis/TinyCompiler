//
// Created by MAC on 2018/12/2.
//

#include "scanner.h"
#include "../data_structure/errors.h"
#include "../data_structure/tables.h"

size_t Scanner::curr_index = 0;
size_s Scanner::line_label = 1;

Scanner::Scanner(string& bf, Tables& tbs): 
    buffer(bf), tables(tbs), p_string(bf), p_number(bf), p_charac(bf) {}

Scanner::~Scanner() = default;

Token Scanner::Scanner::scan_next() {
    Token token;
    char current_char = buffer[curr_index ++];

    // 删除当前位置到下一个单词之间的空白符
    while (is_blank(current_char) && curr_index <= buffer.length()) {
        if (current_char == '\n') line_label ++;
        current_char = buffer[curr_index ++];
    }

    size_t old_index = curr_index - 1;

    // 如果扫描到文件尾，结束扫描
    if (curr_index > buffer.length()) {
        throw ScannerException(line_label, Errors::fake_error[0]);
    }

    // 识别字符，填tables.cT表
    else if (current_char == '\'') {
        char ch;
        if (p_charac.process(curr_index, ch)) {
            int index = find(tables.cT, ch);
            if (index == -1) tables.cT.push_back(&ch);
            token = new Tables::SYNBL_V {
                "", tables.synbl_cur->get_xtp('c'), Tables::CONSTANT, &ch
            };
        } else throw ScannerException(line_label, Errors::syntax_error[0]);
    }

    // 识别字符串，填tables.ST表
    else if (current_char == '\"') {
        string str;
        if (p_string.process(curr_index, str)) {
            int index = find(tables.ST, str);
            if (index == -1) tables.ST.push_back(&str);
            token = new Tables::SYNBL_V {
                "", tables.synbl_cur->get_xtp('s'), Tables::CONSTANT, &str
            };
        } else throw ScannerException(line_label, Errors::syntax_error[1]);
    }

    // 识别算术常数，填tables.CT表
    else if (is_digit(current_char)) {
        auto * num = new Tables::Number;
        Tables::TVAL type;
        curr_index --;
        if (p_number.process(curr_index, num)) {
            int index = find(tables.CT, *num);
            if (index != -1) tables.CT.push_back(num);
            char xtp = num->type == Tables::INTEGER ? 'i': 'r';
            token = new Tables::SYNBL_V {
                "", tables.synbl_cur->get_xtp(xtp), Tables::CONSTANT, num
            };
        } else throw ScannerException(line_label, Errors::syntax_error[2]);
    }

    // 识别用户定义标识符和关键字，查tables.KT表，查填tables.IT表
    else if (is_letter(current_char) or current_char == '_') {
        size_t l_index = curr_index - 1;
        do current_char = buffer[curr_index ++];
        while (is_letter(current_char) or is_digit(current_char) or current_char == '_');
        string tmp = buffer.substr(size_t(l_index), size_t(curr_index - l_index - 1));
        if (tmp == "true" or tmp == "false") {
            Tables::Number num;
            num.type = Tables::BOOLEAN;
            num.value.b = tmp == "true";
            int index = find(tables.CT, num);
            if (index != -1) tables.CT.push_back(&num);
            token = new Tables::SYNBL_V {
                "", tables.synbl_cur->get_xtp('b'), Tables::CONSTANT, &num
            };
        } else {
            if (count(all(tables.KT), tmp) != 0) {
                token = new Tables::SYNBL_V {
                    "", nullptr, Tables::KEYWORD, nullptr
                };
            } else {
                auto * synbl_v = tables.synbl_cur->search(tmp);
                if (synbl_v == nullptr) {
                    token = tables.synbl_cur->add(tmp);
                } else {
                    token = synbl_v;
                }
            }
        }
        curr_index --;
    }

    // 识别界符，查tables.PT表
    else {
        token = new Tables::SYNBL_V {"", nullptr, Tables::DELIMITER, nullptr};
        string delimiter;
        bool has_value = false;
        for (int i = 2; i >= 0; i --) {
            if (curr_index + i > buffer.length()) continue;
            delimiter = buffer.substr(size_t(curr_index - 1), size_t(i + 1));
            if (count(all(tables.PT), delimiter) != 0) {
                has_value = true;
                curr_index += i;
                break;
            }
        }
        if (has_value) {
            if (delimiter == "//") {
                while(curr_index < buffer.length() && buffer[curr_index ++] != '\n');
                line_label ++;
                return scan_next();
            } else if (delimiter == "/*") {
                while(curr_index < buffer.length() - 1 && buffer.substr(size_t(curr_index ++), 2) != "*/")
                    if (buffer[curr_index] == '\n') line_label ++;
                curr_index += 1;
                if (curr_index >= buffer.length())
                    throw ScannerException(line_label, Errors::symbol_error[1]);
                else return scan_next();
            }
        } else throw ScannerException(line_label, Errors::symbol_error[0]);
    }
    token->src = buffer.substr(old_index, curr_index - old_index);
    return token;
}

size_s Scanner::get_line() {
    return line_label;
}
