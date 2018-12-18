//
// Created by MAC on 2018/12/18.
//

#ifndef COMPILE_SEMANTIC_H
#define COMPILE_SEMANTIC_H

#include "../utility/utility.h"
#include "../data_structure/data_structure.h"

class Semantic {
public:

    Tables& tables;

    explicit Semantic(Tables& t);
    ~Semantic();
};

#endif //COMPILE_SEMANTIC_H
