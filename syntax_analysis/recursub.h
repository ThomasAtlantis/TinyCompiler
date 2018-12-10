//
// Created by MAC on 2018/12/10.
//

#ifndef COMPILE_RECURSUB_H
#define COMPILE_RECURSUB_H

#include "../utility/utility.h"
#include "../data_structure/data_structure.h"

class Recursub {
private:
    Grammar& G;
    bool is_available();
    bool subprogram(
            vector<Token>& tokens,
            string left,
            vector<Quarternary>& Qs,
            vector<Tables::Number*>& operands,
            vector<int>& layers
    );
    string token2str(Token token);

public:
    bool available;

    Recursub(Grammar& G);
    ~Recursub();

    vector<Quarternary> check_trans(vector<Token> tokens);
};

#endif //COMPILE_RECURSUB_H
