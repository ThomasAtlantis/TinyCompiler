//
// Created by MAC on 2018/12/2.
//

#include "scanner.h"

int Scanner::curr_index = 0;
int Scanner::line_label = 0;

Scanner::Scanner(string& bf, Tables& tbs): 
    buffer(bf), tables(tbs), p_string(bf), p_number(bf), p_charac(bf) {}

Scanner::~Scanner() = default;

ScannerGet Scanner::scan_next() {
    ScannerGet result;
    char current_char = buffer[curr_index ++];
    while (is_blank(current_char) && curr_index <= buffer.length()) {
        if (current_char == '\n') line_label ++;
        current_char = buffer[curr_index ++];
    }
    //unsigned int old_index = curr_index - 1;
    if (curr_index > buffer.length()) { // 如果扫描到文件尾，结束扫描
        result.error_type = 1;
    } else if (current_char == '\'') { // 识别字符，填tables.cT表
        char ch;
        if (p_charac.process(curr_index, ch)) {
            tables.cT.push_back(ch);
            result.error_type = 0;
            result.token.kind = 'c';
            result.token.index = tables.cT.size() - 1;
        } else {
            result.error_type = -1;
            result.error_log = "Syntax Error: incorrect character";
        }
    } else if (current_char == '\"') { // 识别字符串，填tables.ST表
        string str;
        if (p_string.process(curr_index, str)) {
            tables.ST.push_back(str);
            result.error_type = 0;
            result.token.kind = 'S';
            result.token.index = tables.ST.size() - 1;
        } else {
            result.error_type = -1;
            result.error_log = "Syntax Error: incorrect character string";
        }
    } else if (is_digit(current_char)) { // 识别算术常数，填tables.CT表
        Tables::Number num;
        Tables::Num_type type;
        curr_index --;
        if (p_number.process(curr_index, num, type)) {
            tables.CT.push_back(num);
            result.error_type = 0;
            result.token.kind = 'C';
            result.token.index = tables.CT.size() - 1;
        } else {
            result.error_type = -1;
            result.error_log = "Syntax Error: incorrect constant";
        }
    } else if (is_letter(current_char) or current_char == '_') { // 识别用户定义标识符和关键字，查tables.KT表，查填tables.IT表
        int l_index = curr_index - 1;
        do current_char = buffer[curr_index ++];
        while (is_letter(current_char) or is_digit(current_char) or current_char == '_');
        string tmp = buffer.substr(l_index, curr_index - l_index - 1);
        auto index = find(all(tables.KT), tmp);
        if (index != tables.KT.end()) {
            result.error_type = 0;
            result.token.kind = 'K';
            result.token.index = index - tables.KT.begin();
        } else {
            if (!tables.IT.empty())
                index = find(all(tables.IT), tmp);
            result.error_type = 0;
            result.token.kind = 'I';
            if (tables.IT.empty() || index == tables.IT.end()) {
                tables.IT.push_back(tmp);
                result.token.index = tables.IT.size() - 1;
            } else {
                result.token.index = index - tables.IT.begin();
            }
        }
        curr_index --;
    } else { // 识别界符，查tables.PT表
        result.error_type = -1; result.token.kind = 'P';
        result.error_log = "Symbol Error: incorrect delimiters";
        for (int i = 0; i < 3; i ++) {
            if (curr_index + i > buffer.length()) break;
            string key = buffer.substr(curr_index - 1, i + 1);
            auto index = find(all(tables.PT), key);
            if (index != tables.PT.end()) {
                result.error_type = 0;
                result.token.index = index - tables.PT.begin();
            } else break;
        }
        if (!result.error_type) {
            if (tables.PT[result.token.index] == "//") {
                while(curr_index < buffer.length() && buffer[curr_index ++] != '\n');
                line_label ++;
                result.error_type = 1;
                return result;
            } else if (tables.PT[result.token.index] == "/*") {
                while(curr_index < buffer.length() - 1 && buffer.substr(curr_index ++, 2) != "*/") {
                    if (buffer[curr_index] == '\n') line_label ++;
                }
                curr_index += 1;
                if (curr_index >= buffer.length()) {
                    result.error_type = -1;
                    result.error_log = "Symbol Error: incorrect annotation";
                } else result.error_type = 1;
                return result;
            }
        }
    }
//    if (result.error_type == 0)
//        cout << left << "[" << setw(4) << line_label << "] " << buffer.substr(old_index, curr_index - old_index) << " ";
    return result;
}