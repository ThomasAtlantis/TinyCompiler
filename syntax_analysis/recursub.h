//
// Created by MAC on 2018/12/10.
//

#ifndef COMPILE_RECURSUB_H
#define COMPILE_RECURSUB_H

#include <utility>
#include "../utility/utility.h"
#include "../data_structure/data_structure.h"
#include "../lexical_analysis/scanner.h"

class Recursub {
private:
    Grammar& G;
    Scanner& scanner;
    string w;
    Token token;
    vector<Tables::Number*> operands;
    vector<Quarternary> Qs;
    bool is_available();
    bool subprogram(string left);
    string token2str(Token token);

public:
    bool available;

    Recursub(Grammar& G, Scanner& scanner);
    ~Recursub();

    vector<Quarternary> check_trans();
};

#endif //COMPILE_RECURSUB_H
