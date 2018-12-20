#include "data_structure/grammar.h"
#include "utility/utility.h"
#include "lexical_analysis/scanner.h"
#include "syntax_analysis/LL_1.h"

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
    grammar.add("A0") >> "A1 A2";
    grammar.add("A1") >> "A3 A1" | "null";
    grammar.add("A3") >> "A4 qua_sav_func_name ( A5 ) { qua_new_synbl A6 }";
    grammar.add("A5") >> "A7 A8" | "null";
    grammar.add("A8") >> ", A7 A8" | "null";
    grammar.add("A7") >> "A9 B0";
    grammar.add("B0") >> "A4" | "& A4";
    grammar.add("A2") >> "main qua_sav_func_name ( ) { qua_new_synbl A6 }";
    grammar.add("A6") >> "B1 A6" | "null" | "B2 A6";
    grammar.add("B1") >> "B3" | "B4" | "B5";
    grammar.add("B3") >> "B6 ;";
    grammar.add("B5") >> "if ( B7 ) { A6 } B8";
    grammar.add("B8") >> "else B9" | "null";
    grammar.add("B9") >> "{ A6 }" | "B5";
    grammar.add("B4") >> "while ( B7 ) { A6 }";
    grammar.add("B4") >> "for ( C0 ; C1 ; C2 ) { A6 }";
    grammar.add("C0") >> "B6" | "null";
    grammar.add("C1") >> "B7" | "null";
    grammar.add("C2") >> "B6" | "null";
    grammar.add("B2") >> "continue ;" | "break ;";
    grammar.add("C3") >> "putc C4" | "puts C5";
    grammar.add("C4") >> "A4" | "C6";
    grammar.add("C7") >> "C8" | "C9";
    grammar.add("C8") >> "D0 = B7";
    grammar.add("C9") >> "( D1 )";
    grammar.add("D1") >> "D2 D3" | "null";
    grammar.add("D3") >> ", D2 D3" | "null";
    grammar.add("D2") >> "A4" | "D4";
    grammar.add("D5") >> "return";
    grammar.add("A9") >> "int qua_int" | "float qua_float" | "bool qua_bool" | "char qua_char";
    grammar.add("B7") >> "D6";
    grammar.add("D6") >> "D7 D8";
    grammar.add("D8") >> "> D7" | ">= D7" | "< D7" | "<= D7" | "== D7" | "!= D7" | "null";
    grammar.add("D7") >> "D9";
    grammar.add("D9") >> "E0";
    grammar.add("E0") >> "E1 E2";
    grammar.add("E2") >> "+ E1 qua+ E2" | "- E1 qua- E2" | "null";
    grammar.add("E1") >> "E3 E4";
    grammar.add("E4") >> "* E3 qua* E4" | "/ E3 qua/ E4" | "null";
    grammar.add("E3") >> "E5";
    grammar.add("E5") >> "E6" | "! E6" | "- E6 qua.";
    grammar.add("E6") >> "A4 quap D0" | "D4 quap" | "( B7 )";
    grammar.add("B6") >> "E7" | "A4 C7" | "return" | "C3" | "struct A4 E8";
    grammar.add("E7") >> "A9 E9";
    grammar.add("E9") >> "A4 qua_declare_id F0 qua_declare F1";
    grammar.add("F0") >> "[ F2 qua_declare_arr ]" | "null";
    grammar.add("F1") >> "= B7" | "null";
    grammar.add("E8") >> "A4" | "{ F3 ; F4 }";
    grammar.add("F3") >> "A9 A4 D0" | "struct A4 A4";
    grammar.add("F4") >> "F3 ; F4" | "null";
    grammar.add("A4") >> "@I";
    grammar.add("F5") >> "F6" | "F2";
    grammar.add("D0") >> "[ F5 ]" | "null";
    grammar.add("D4") >> "F2" | "F7" | "F8";
    grammar.add("F2") >> "@INT";
    grammar.add("F7") >> "@FLT";
    grammar.add("C6") >> "@CH";
    grammar.add("C5") >> "@STR";
    grammar.add("F8") >> "true" | "false";
    //*/

    const string file_name = "../source.cmm";
    ifstream source_file(file_name.c_str());
    if (!source_file.is_open()) {
        cout << Errors::file_error[0] << endl;
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
        } catch (exception& e) {
            cout << e.what() << endl;
        }
    }
    return 0;
}