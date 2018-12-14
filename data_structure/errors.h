//
// Created by MAC on 2018/12/4.
//

#ifndef COMPILE_ERRORS_H
#define COMPILE_ERRORS_H

#include "../utility/utility.h"

class Errors {
public:
    typedef enum {error=-1, clear=0, eof=1} Type;

    typedef struct {
        Type type;
        string log;
    } Error_message;

    static const string syntax_error[];

    static const string symbol_error[];
};

class ScannerException: public exception {
private:
    int line_label;
    string log;
public:
    ScannerException(int line_label, string log) {
        this->line_label = line_label;
        this->log = std::move(log);
    }
    ~ScannerException() noexcept override = default;

    const char* what() const noexcept override {
        return ("(" + to_string(line_label) + ") " + log).c_str();
    }
};

class SyntaxException: public exception {
private:
    int line_label;
    string log;
public:
    SyntaxException(int line_label, string log) {
        this->line_label = line_label;
        this->log = std::move(log);
    }
    ~SyntaxException() noexcept override = default;

    const char* what() const noexcept override {
        return ("(" + to_string(line_label) + ") " + log).c_str();
    }
};

#endif //COMPILE_ERRORS_H
