//
// Created by MAC on 2018/12/2.
//

#include "scanner.h"
#include "../data_structure/errors.h"

int Scanner::curr_index = 0;
int Scanner::line_label = 1;

Scanner::Scanner(string& bf, Tables& tbs): 
    buffer(bf), tables(tbs), p_string(bf), p_number(bf), p_charac(bf) {}

Scanner::~Scanner() = default;

Scanner::Scanner_ret Scanner::Scanner::scan_next() {
    Scanner_ret result;
    char current_char = buffer[curr_index ++];

    // 删除当前位置到下一个单词之间的空白符
    while (is_blank(current_char) && curr_index <= buffer.length()) {
        if (current_char == '\n') line_label ++;
        current_char = buffer[curr_index ++];
    }

    // 如果扫描到文件尾，结束扫描
    if (curr_index > buffer.length()) {
        result.error_m.type = Errors::eof;
    }

    // 识别字符，填tables.cT表
    else if (current_char == '\'') {
        char ch;
        if (p_charac.process(curr_index, ch)) {
            tables.cT.push_back(ch);
            result.error_m.type = Errors::clear;
            result.token.kind = 'c';
            result.token.index = tables.cT.size() - 1;
        } else {
            result.error_m.type = Errors::error;
            result.error_m.log = Errors::syntax_error[0];
        }
    }

    // 识别字符串，填tables.ST表
    else if (current_char == '\"') {
        string str;
        if (p_string.process(curr_index, str)) {
            tables.ST.push_back(str);
            result.error_m.type = Errors::clear;
            result.token.kind = 'S';
            result.token.index = tables.ST.size() - 1;
        } else {
            result.error_m.type = Errors::error;
            result.error_m.log = Errors::syntax_error[1];
        }
    }

    // 识别算术常数，填tables.CT表
    else if (is_digit(current_char)) {
        Tables::Number num;
        Tables::Num_type type;
        curr_index --;
        if (p_number.process(curr_index, num, type)) {
            tables.CT.push_back(num);
            result.error_m.type = Errors::clear;
            result.token.kind = 'C';
            result.token.index = tables.CT.size() - 1;
        } else {
            result.error_m.type = Errors::error;
            result.error_m.log = Errors::syntax_error[2];
        }
    }

    // 识别用户定义标识符和关键字，查tables.KT表，查填tables.IT表
    else if (is_letter(current_char) or current_char == '_') {
        int l_index = curr_index - 1;
        do current_char = buffer[curr_index ++];
        while (is_letter(current_char) or is_digit(current_char) or current_char == '_');
        string tmp = buffer.substr(size_t(l_index), size_t(curr_index - l_index - 1));
        auto index = find(all(tables.KT), tmp);
        if (index != tables.KT.end()) {
            result.error_m.type = Errors::clear;
            result.token.kind = 'K';
            result.token.index = size_t(index - tables.KT.begin());
        } else {
            if (!tables.IT.empty())
                index = find(all(tables.IT), tmp);
            result.error_m.type = Errors::clear;
            result.token.kind = 'I';
            if (tables.IT.empty() || index == tables.IT.end()) {
                tables.IT.push_back(tmp);
                result.token.index = tables.IT.size() - 1;
            } else {
                result.token.index = size_t(index - tables.IT.begin());
            }
        }
        curr_index --;
    }

    // 识别界符，查tables.PT表
    else {
        result.error_m.type = Errors::error; result.token.kind = 'P';
        result.error_m.log = Errors::symbol_error[0];
        for (int i = 0; i < 3; i ++) {
            if (curr_index + i > buffer.length()) break;
            string key = buffer.substr(size_t(curr_index - 1), size_t(i + 1));
            auto index = find(all(tables.PT), key);
            if (index != tables.PT.end()) {
                result.error_m.type = Errors::clear;
                result.token.index = size_t(index - tables.PT.begin());
            } else break;
        }
        if (result.error_m.type == Errors::clear) {
            if (tables.PT[result.token.index] == "//") {
                while(curr_index < buffer.length() && buffer[curr_index ++] != '\n');
                line_label ++;
                return scan_next();
            } else if (tables.PT[result.token.index] == "/*") {
                while(curr_index < buffer.length() - 1 && buffer.substr(size_t(curr_index ++), 2) != "*/")
                    if (buffer[curr_index] == '\n') line_label ++;
                curr_index += 1;
                if (curr_index >= buffer.length()) {
                    result.error_m.type = Errors::error;
                    result.error_m.log = Errors::symbol_error[1];
                    return result;
                } else return scan_next();
            }
        }
    }
//    if (result.error_type == 0)
//        cout << left << "[" << setw(4) << line_label << "] " << buffer.substr(old_index, curr_index - old_index) << " ";
    return result;
}

int Scanner::get_line() {
    return line_label;
}
