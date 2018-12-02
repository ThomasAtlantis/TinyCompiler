#include "data_structure/grammar.h"
#include "utility/utility.h"

int main() {
    Tables tables;
    Grammar grammar(tables);
    grammar.add("E") >> "F T1";
    grammar.add("T1") >> "+ F qua+ T1" | "- F qua- T1" | "epsilon";
    grammar.add("F") >> "H F1";
    grammar.add("F1") >> "* H qua* F1" | "/ H qua/ F1" | "epsilon";
    grammar.add("H") >> "H1" | "- H1 qua.";
    grammar.add("H1") >> "K1 J";
    grammar.add("J") >> "^ K1 qua^" | "epsilon";
    grammar.add("K2") >> "- K1 qua." | "K1";
    grammar.add("K1") >> "/I quap" | "/C quap" | "( E )";
    cout << grammar;
    cout << grammar.first_set_of({"E"}) << endl;
    cout << grammar.select_set_of("F", split("H F1", " ")) << endl;
    return 0;
}