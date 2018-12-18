#include "data_structure/grammar.h"
#include "utility/utility.h"
#include "lexical_analysis/scanner.h"
#include "syntax_analysis/recursub.h"
#include "syntax_analysis/LL_1.h"
#include "data_structure/errors.h"

int main() {
    Tables tables;
    Grammar grammar(tables);
    /*
    grammar.set_start("E");
    grammar.add("E") >> "F T1";
    grammar.add("T1") >> "+ F qua+ T1" | "- F qua- T1" | "null";
    grammar.add("F") >> "H F1";
    grammar.add("F1") >> "* H qua* F1" | "/ H qua/ F1" | "null";
    grammar.add("H") >> "H1" | "- H1 qua.";
    grammar.add("H1") >> "K1 J";
    grammar.add("J") >> "^ K1 qua^" | "null";
    grammar.add("K2") >> "- K1 qua." | "K1";
    grammar.add("K1") >> "/I quap" | "/C quap" | "( E )";
    //*/
    //*
    grammar.set_start("A0");
    grammar.add("A0") >> "A1";
    grammar.add("A1") >> "void main ( ) { A2 }";
    grammar.add("A2") >> "A3 A2" | "null" | "A4 A2";
    grammar.add("A3") >> "A5" | "A6" | "A7";
    grammar.add("A5") >> "A8 ;";
    grammar.add("A7") >> "if ( A9 ) { A2 } B0";
    grammar.add("B0") >> "else B1" | "null";
    grammar.add("B1") >> "{ A2 }" | "A7";
    grammar.add("A6") >> "while ( A9 ) { A2 }";
    grammar.add("A6") >> "for ( B2 ; B3 ; B4 ) { A2 }";
    grammar.add("B2") >> "A8" | "null";
    grammar.add("B3") >> "A9" | "null";
    grammar.add("B4") >> "A8" | "null";
    grammar.add("A4") >> "continue ;" | "break ;";
    grammar.add("B5") >> "putc B6" | "puts B7";
    grammar.add("B6") >> "B8" | "B9";
    grammar.add("C0") >> "= A9" | "null";
    grammar.add("C1") >> "C2 = A9";
    grammar.add("C3") >> "return";
    grammar.add("C4") >> "int" | "float" | "bool" | "char";
    grammar.add("C5") >> "@C";
    grammar.add("C2") >> "B8 C6";
    grammar.add("A9") >> "C7";
    grammar.add("C7") >> "C8 C9";
    grammar.add("C9") >> "> C8" | ">= C8" | "< C8" | "<= C8" | "== C8" | "!= C8" | "null";
    grammar.add("C8") >> "D0";
    grammar.add("D0") >> "D1";
    grammar.add("D1") >> "D2 D3";
    grammar.add("D3") >> "+ D2 qua+ D3" | "- D2 qua- D3" | "null";
    grammar.add("D2") >> "D4 D5";
    grammar.add("D5") >> "* D4 qua* D5" | "/ D4 qua/ D5" | "null";
    grammar.add("D4") >> "D6";
    grammar.add("D6") >> "D7" | "! D7" | "- D7";
    grammar.add("D7") >> "B8 C6" | "C5 quap" | "( A9 )";
    grammar.add("A8") >> "D8" | "C1" | "return" | "B5" | "struct B8 D9";
    grammar.add("D8") >> "C4 E0";
    grammar.add("E0") >> "B8 E1";
    grammar.add("E1") >> "C0" | "[ E2 ]";
    grammar.add("D9") >> "B8" | "{ E3 ; E4 }";
    grammar.add("E3") >> "C4 B8 C6" | "struct B8 B8";
    grammar.add("E4") >> "E3 ; E4" | "null";
    grammar.add("B8") >> "@I";
    grammar.add("E2") >> "E5" | "E6";
    grammar.add("C6") >> "[ E2 ]" | "null";
    grammar.add("C5") >> "E6" | "E7" | "E8";
    grammar.add("E6") >> "@INT";
    grammar.add("E7") >> "@FLT";
    grammar.add("B9") >> "@CH";
    grammar.add("B7") >> "@STR";
    grammar.add("E8") >> "true" | "false";
    //*/
    const string file_name = "../source.cmm";
    ifstream source_file(file_name.c_str());
    if (!source_file.is_open()) {
        cout << "File Error: failed to open source file" << endl;
        return 0;
    }
    string buffer((std::istreambuf_iterator<char>(source_file)), std::istreambuf_iterator<char>());
    source_file.close();

    Scanner scanner(buffer, tables);
    LL1 ll1(grammar, scanner);
    if (ll1.available) {
        try {
            vector<Quarternary> Qs = ll1.check_trans();
            if (!Qs.empty()) print_qs(Qs);
        } catch (SyntaxException& e) {
            cout << e.what() << endl;
        }
    }
    return 0;
}