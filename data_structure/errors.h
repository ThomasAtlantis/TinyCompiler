//
// Created by MAC on 2018/12/4.
//

#ifndef COMPILE_ERRORS_H
#define COMPILE_ERRORS_H

#include "../utility/utility.h"
#include "tables.h"
// TODO: 报错时要rewind到上一个token
class Errors {
public:
    static const string syntax_error[];
    static const string symbol_error[];
    static const string fake_error[];
    static const string file_error[];
};

class ScannerException: public exception {
private:
    size_s line_label;
    string log;
public:
    ScannerException(size_s line_label, string log) noexcept {
        this->line_label = line_label;
        this->log = std::move(log);
    }
    ~ScannerException() noexcept override = default;

    size_s get_line() noexcept {
        return this->line_label;
    }

    string get_log() noexcept {
        return this->log;
    }

    const char* what() const noexcept override {
        return ("(" + to_string(line_label) + ") " + log).c_str();
    }
};

class SyntaxException: public exception {
private:
    size_s line_label;
    string log;
public:
    SyntaxException(size_s line_label, string log) noexcept {
        this->line_label = line_label;
        this->log = std::move(log);
    }

    ~SyntaxException() noexcept override = default;

    const char* what() const noexcept override {
        return ("(" + to_string(line_label) + ") " + log).c_str();
    }
};

#endif //COMPILE_ERRORS_H
