#include "data_structure/grammar.h"
#include "utility/utility.h"
#include "lexical_analysis/scanner.h"
#include "syntax_analysis/LL_1.h"
#include "data_structure/tables.h"

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
    grammar.add("A1") >> "A3 A1" | "A4 A1" | "null";
    grammar.add("A4") >> "A5 ;";
    grammar.add("A5") >> "A6" | "A7";
    grammar.add("A3") >> "A8 qua_sav_func_name qua_new_synbl qua_new_pfinfl ( A9 ) { B0 qua_endproc qua_return }";
    grammar.add("A9") >> "B1 B2" | "null";
    grammar.add("B2") >> ", B1 B2" | "null";
    grammar.add("B1") >> "B3 B4 qua_declare qua_declare_param";
    grammar.add("B4") >> "A8 qua_declare_id B5" | "& qua_addrParm A8 qua_declare_id";
    grammar.add("A2") >> "main qua_sav_func_name ( ) { qua_new_synbl B0 qua_return }";
    grammar.add("B0") >> "B6 B0" | "null" | "B7 B0";
    grammar.add("B6") >> "B8" | "B9" | "C0";
    grammar.add("B8") >> "C1 ;";
    grammar.add("C0") >> "if qua_gen_func_name qua_new_synbl ( C2 ) quaif { B0 qua_endif qua_return } C3";
    grammar.add("C3") >> "else qua_gen_func_name C4" | "null";
    grammar.add("C4") >> "{ qua_new_synbl B0 qua_endifall qua_return }" | "C0";
    grammar.add("B9") >> "while qua_gen_func_name qua_new_synbl quawh ( C2 ) { quawdo B0 quawe qua_return }";
    grammar.add("B9") >> "for qua_gen_func_name qua_new_synbl ( C5 ; quafor C6 quafdo ; C7 quafsav ) { B0 quafe qua_return }";
    grammar.add("C5") >> "C1" | "null";
    grammar.add("C6") >> "C2" | "null";
    grammar.add("C7") >> "C1" | "null";
    grammar.add("B7") >> "continue qua_continue qua_check_jump ;" | "break qua_break qua_check_jump ;";
    grammar.add("C8") >> "getc A8 qua_getc qua_check_def_e quad";
    grammar.add("C9") >> "putc D0 qua_putc qua_check_def_e quad" | "puts D1 qua_puts";
    grammar.add("D0") >> "A8" | "D2";
    grammar.add("D3") >> "D4" | "D5";
    grammar.add("D4") >> "D6 = C2 qua=" | ". A8 quap qua_member = C2 qua=";
    grammar.add("D5") >> "( qua_call D7 qua_checknum )";
    grammar.add("D7") >> "D8 D9" | "null";
    grammar.add("D9") >> ", D8 D9" | "null";
    grammar.add("D8") >> "A8 quap qua_check_def_e D6 qua_contrast" | "E0 qua_contrastC";
    grammar.add("E1") >> "return";
    grammar.add("B3") >> "int qua_int" | "float qua_float" | "bool qua_bool" | "char qua_char";
    grammar.add("C2") >> "E2";
    grammar.add("E2") >> "E3 E4";
    grammar.add("E4") >> "> E3 qua>" | ">= E3 qua>=" | "< E3 qua<" | "<= E3 qua<=";
    grammar.add("E4") >> "== E3 qua==" | "!= E3 qua!=" | "null";
    grammar.add("E3") >> "E5";
    grammar.add("E5") >> "E6";
    grammar.add("E6") >> "E7 E8";
    grammar.add("E8") >> "+ E7 qua+ E8" | "- E7 qua- E8" | "null";
    grammar.add("E7") >> "E9 F0";
    grammar.add("F0") >> "* E9 qua* F0" | "/ E9 qua/ F0" | "null";
    grammar.add("E9") >> "F1";
    grammar.add("F1") >> "F2" | "! F2" | "- F2 qua.";
    grammar.add("F2") >> "A8 quap qua_check_def_e F3" | "E0 quap" | "( C2 )";
    grammar.add("F3") >> "D6" | ". A8 quap qua_member";
    grammar.add("C1") >> "A6" | "A8 quap qua_check_def_e D3" | "return qua_func_return" | "C9" | "C8" | "A7";
    grammar.add("A7") >> "struct qua_gen_func_name A8 qua_declare_struct_id F4";
    grammar.add("F4") >> "F5" | "F6";
    grammar.add("F5") >> "A8 qua_struct_check_def qua_struct_declare";
    grammar.add("F6") >> "{ qua_struct_def qua_new_synbl_struct F7 ; F8 qua_return }";
    grammar.add("A6") >> "B3 F9";
    grammar.add("F9") >> "A8 qua_declare_id quap_id G0";
    grammar.add("G0") >> "B5 qua_declare" | "G1 G2";
    grammar.add("B5") >> "[ G3 qua_declare_arr ]" | "null";
    grammar.add("G2") >> "= C2 qua=";
    grammar.add("G1") >> "null qua_declare";
    grammar.add("F7") >> "B3 A8 qua_declare_id B5 qua_instruct_declare";
    grammar.add("F8") >> "F7 ; F8" | "null";
    grammar.add("A8") >> "@I";
    grammar.add("G4") >> "A8 quap qua_check_def_e" | "G3 quap";
    grammar.add("D6") >> "[ G4 ] qua[]" | "null";
    grammar.add("E0") >> "G3" | "G5" | "G6" | "D2" | "D1";
    grammar.add("G3") >> "@INT";
    grammar.add("G5") >> "@FLT";
    grammar.add("D2") >> "@CH";
    grammar.add("D1") >> "@STR";
    grammar.add("G6") >> "true" | "false";

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
    vector<Quarternary> Qs;
    cout << endl << "# Token Sequence #" << endl;
    if (ll1.available) {
        try {
             Qs = ll1.check_trans();
        } catch (exception& e) {
            cout << e.what() << endl;
        }
    }
    size_t i;
    cout << endl << "# Keyword Table #" << endl;
    for (i = 0; i < tables.KT.size(); i ++) {
        cout << "[" << setw(4) << left << i << "] ";
        cout << setw(10) << left << tables.KT[i];
        if (i % 4 == 3) cout << endl;
    }
    if (i % 4 != 0) cout << endl;
    cout << endl << "# Delimiter Table #" << endl;
    for (i = 0; i < tables.PT.size(); i ++) {
        cout << "[" << setw(4) << left << i << "] ";
        cout << setw(10) << left << tables.PT[i];
        if (i % 4 == 3) cout << endl;
    }
    if (i % 4 != 0) cout << endl;
    cout << endl << "# Character Table #" << endl;
    for (i = 0; i < tables.cT.size(); i ++) {
        cout << "[" << setw(4) << left << i << "] ";
        cout << setw(10) << left << string("\'") + *tables.cT[i] + "\'";
        if (i % 4 == 3) cout << endl;
    }
    if (i % 4 != 0) cout << endl;
    cout << endl << "# String Table #" << endl;
    for (i = 0; i < tables.ST.size(); i ++) {
        cout << "[" << setw(4) << left << i << "] ";
        cout << setw(10) << left << string("\"") + tables.ST[i] + "\"";
        if (i % 4 == 3) cout << endl;
    }
    if (i % 4 != 0) cout << endl;
    cout << endl << "# Number Table #" << endl;
    for (i = 0; i < tables.CT.size(); i ++) {
        cout << "[" << setw(4) << left << i << "] ";
        cout << setw(10) << left << *tables.CT[i];
        if (i % 4 == 3) cout << endl;
    }
    if (i % 4 != 0) cout << endl;
    cout << endl << "# Symbol Table #" << endl;
    vector<bool> vec;
    (tables.search_func("#GLOBAL"))->show_tree(vec);
    cout << endl << "# Quarternaries #" << endl;
    if (!Qs.empty()) print_qs(Qs);
    return 0;
}