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
            tables.cT.push_back(ch);
            token.kind = "c";
            token.index = tables.cT.size() - 1;
        } else throw ScannerException(line_label, Errors::syntax_error[0]);
    }

    // 识别字符串，填tables.ST表
    else if (current_char == '\"') {
        string str;
        if (p_string.process(curr_index, str)) {
            tables.ST.push_back(str);
            token.kind = "S";
            token.index = tables.ST.size() - 1;
        } else throw ScannerException(line_label, Errors::syntax_error[1]);
    }

    // 识别算术常数，填tables.CT表
    else if (is_digit(current_char)) {
        auto * num = new Tables::Number;
        Tables::TVAL type;
        curr_index --;
        if (p_number.process(curr_index, num, type)) {
            tables.CT.push_back(num);
            token.kind = "C";
            token.index = tables.CT.size() - 1;
        } else throw ScannerException(line_label, Errors::syntax_error[2]);
    }

    // 识别用户定义标识符和关键字，查tables.KT表，查填tables.IT表
    else if (is_letter(current_char) or current_char == '_') {
        size_t l_index = curr_index - 1;
        do current_char = buffer[curr_index ++];
        while (is_letter(current_char) or is_digit(current_char) or current_char == '_');
        string tmp = buffer.substr(size_t(l_index), size_t(curr_index - l_index - 1));
        if (tmp == "true" or tmp == "false") {
            token.kind = "C";
            token.index = tables.CT.size();
            Tables::Number num;
            num.type = Tables::BOOLEAN;
            num.value.b = tmp == "true";
            tables.CT.push_back(&num);
        } else {
            auto index = find(all(tables.KT), tmp);
            if (index != tables.KT.end()) {
                token.kind = "K";
                token.index = size_t(index - tables.KT.begin());
            } else {
                token.kind = "I";
                token.index = 0;
                auto * synbl_v = tables.search(tables.synbl_cur->child, tmp);
                if (synbl_v == nullptr) {
                    tables.add(tables.synbl_cur->child, tmp);
                }
            }
        }
        curr_index --;
    }

    // 识别界符，查tables.PT表
    else {
        bool has_value = false;
        token.kind = "P";
        for (int i = 2; i >= 0; i --) {
            if (curr_index + i > buffer.length()) continue;
            string key = buffer.substr(size_t(curr_index - 1), size_t(i + 1));
            auto index = find(all(tables.PT), key);
            if (index != tables.PT.end()) {
                has_value = true;
                token.index = size_t(index - tables.PT.begin());
                curr_index += i;
                break;
            }
        }
        if (has_value) {
            if (tables.PT[token.index] == "//") {
                while(curr_index < buffer.length() && buffer[curr_index ++] != '\n');
                line_label ++;
                return scan_next();
            } else if (tables.PT[token.index] == "/*") {
                while(curr_index < buffer.length() - 1 && buffer.substr(size_t(curr_index ++), 2) != "*/")
                    if (buffer[curr_index] == '\n') line_label ++;
                curr_index += 1;
                if (curr_index >= buffer.length())
                    throw ScannerException(line_label, Errors::symbol_error[1]);
                else return scan_next();
            }
        } else throw ScannerException(line_label, Errors::symbol_error[0]);
    }
    token.src = buffer.substr(old_index, curr_index - old_index);
    return token;
}

size_s Scanner::get_line() {
    return line_label;
}
