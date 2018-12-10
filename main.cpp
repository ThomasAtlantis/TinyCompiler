#include "data_structure/grammar.h"
#include "utility/utility.h"
#include "lexical_analysis/scanner.h"
#include "syntax_analysis/recursub.h"
#include "data_structure/errors.h"

int main() {
    Tables tables;
    Grammar grammar(tables);
    grammar.set_start("E");
    grammar.add("E") >> "F T1";
    grammar.add("T1") >> "+ F qua+ T1" | "- F qua- T1" | "epsilon";
    grammar.add("F") >> "H F1";
    grammar.add("F1") >> "* H qua* F1" | "/ H qua/ F1" | "epsilon";
    grammar.add("H") >> "H1" | "- H1 qua.";
    grammar.add("H1") >> "K1 J";
    grammar.add("J") >> "^ K1 qua^" | "epsilon";
    grammar.add("K2") >> "- K1 qua." | "K1";
    grammar.add("K1") >> "/I quap" | "/C quap" | "( E )";

    const string file_name = "../source.cmm";
    ifstream source_file(file_name.c_str());
    if (!source_file.is_open()) {
        cout << "File Error: failed to open source file" << endl;
        return 0;
    }
    string buffer((std::istreambuf_iterator<char>(source_file)), std::istreambuf_iterator<char>());
    source_file.close();

    Scanner scanner(buffer, tables);
    Scanner::Scanner_ret sg;
    vector<Token> tokens;
    sg = scanner.scan_next();
    while (sg.error_m.type != Errors::eof) {
        if (sg.error_m.type == Errors::error) {
            cout << file_name << "(" << scanner.get_line() << "): " << sg.error_m.log << endl;
            return 0;
        } else if (sg.error_m.type == Errors::clear) {
            tokens.push_back(sg.token);
        }
        sg = scanner.scan_next();
    }

    Recursub recursub(grammar);
    if (recursub.available) {
        vector<Quarternary> Qs = recursub.check_trans(tokens);
        print_qs(Qs);
    }

    return 0;
}