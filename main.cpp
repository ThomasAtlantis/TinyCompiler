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
    grammar.add("A3") >> "A4 qua_sav_func_name qua_new_synbl ( A5 ) { A6 qua_return }";
    grammar.add("A5") >> "A7 A8" | "null";
    grammar.add("A8") >> ", A7 A8" | "null";
    grammar.add("A7") >> "A9 B0 qua_declare";
    grammar.add("B0") >> "A4 qua_declare_id B1" | "& A4 qua_declare_id";
    grammar.add("A2") >> "main qua_sav_func_name ( ) { qua_new_synbl A6 qua_return }";
    grammar.add("A6") >> "B2 A6" | "null" | "B3 A6";
    grammar.add("B2") >> "B4" | "B5" | "B6";
    grammar.add("B4") >> "B7 ;";
    grammar.add("B6") >> "if qua_gen_func_name qua_new_synbl ( B8 ) quaif { A6 qua_endif qua_return } B9";
    grammar.add("B9") >> "else qua_gen_func_name C0" | "null";
    grammar.add("C0") >> "{ qua_new_synbl A6 qua_endifall qua_return }" | "B6";
    grammar.add("B5") >> "while qua_gen_func_name qua_new_synbl quawh ( B8 ) { quawdo A6 quawe qua_return }";
    grammar.add("B5") >> "for qua_gen_func_name qua_new_synbl ( C1 ; quafor C2 quafdo ; C3 quafsav ) { A6 quafe qua_return }";
    grammar.add("C1") >> "B7" | "null";
    grammar.add("C2") >> "B8" | "null";
    grammar.add("C3") >> "B7" | "null";
    grammar.add("B3") >> "continue qua_continue qua_check_jump ;" | "break qua_break qua_check_jump ;";
    grammar.add("C4") >> "getc A4 qua_getc qua_check_def_e quad";
    grammar.add("C5") >> "putc C6 qua_putc qua_check_def_e quad" | "puts C7 qua_puts";
    grammar.add("C6") >> "A4" | "C8";
    grammar.add("C9") >> "D0" | "D1";
    grammar.add("D0") >> "D2 = B8 qua=" | ". A4 quap qua_member = B8 qua=";
    grammar.add("D1") >> "( D3 )";
    grammar.add("D3") >> "D4 D5" | "null";
    grammar.add("D5") >> ", D4 D5" | "null";
    grammar.add("D4") >> "A4 quap qua_check_def_e D2" | "D6";
    grammar.add("D7") >> "return";
    grammar.add("A9") >> "int qua_int" | "float qua_float" | "bool qua_bool" | "char qua_char";
    grammar.add("B8") >> "D8";
    grammar.add("D8") >> "D9 E0";
    grammar.add("E0") >> "> D9 qua>" | ">= D9 qua>=" | "< D9 qua<" | "<= D9 qua<=";
    grammar.add("E0") >> "== D9 qua==" | "!= D9 qua!=" | "null";
    grammar.add("D9") >> "E1";
    grammar.add("E1") >> "E2";
    grammar.add("E2") >> "E3 E4";
    grammar.add("E4") >> "+ E3 qua+ E4" | "- E3 qua- E4" | "null";
    grammar.add("E3") >> "E5 E6";
    grammar.add("E6") >> "* E5 qua* E6" | "/ E5 qua/ E6" | "null";
    grammar.add("E5") >> "E7";
    grammar.add("E7") >> "E8" | "! E8" | "- E8 qua.";
    grammar.add("E8") >> "A4 quap qua_check_def_e E9" | "D6 quap" | "( B8 )";
    grammar.add("E9") >> "D2" | ". A4 quap qua_member";
    grammar.add("B7") >> "F0" | "A4 quap qua_check_def_e C9" | "return" | "C5" | "C4";
    grammar.add("B7") >> "struct qua_gen_func_name A4 qua_declare_struct_id F1";
    grammar.add("F1") >> "A4 qua_struct_check_def qua_struct_declare";
    grammar.add("F1") >> "{ qua_struct_def qua_new_synbl_struct F2 ; F3 qua_return }";
    grammar.add("F0") >> "A9 F4";
    grammar.add("F4") >> "A4 qua_declare_id quap_id F5";
    grammar.add("F5") >> "B1 qua_declare" | "F6 F7";
    grammar.add("B1") >> "[ F8 qua_declare_arr ]" | "null";
    grammar.add("F7") >> "= B8 qua=";
    grammar.add("F6") >> "null qua_declare";
    grammar.add("F2") >> "A9 A4 qua_declare_id B1 qua_instruct_declare";
    grammar.add("F3") >> "F2 ; F3" | "null";
    grammar.add("A4") >> "@I";
    grammar.add("F9") >> "A4 quap qua_check_def_e" | "F8 quap";
    grammar.add("D2") >> "[ F9 ] qua[]" | "null";
    grammar.add("D6") >> "F8" | "G0" | "G1" | "C8" | "C7";
    grammar.add("F8") >> "@INT";
    grammar.add("G0") >> "@FLT";
    grammar.add("C8") >> "@CH";
    grammar.add("C7") >> "@STR";
    grammar.add("G1") >> "true" | "false";

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