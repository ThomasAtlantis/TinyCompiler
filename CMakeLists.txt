cmake_minimum_required(VERSION 3.6)
project(Compile)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)

add_executable(Compile main.cpp data_structure/grammar.cpp data_structure/grammar.h utility/utility.cpp utility/utility.h data_structure/tables.cpp data_structure/tables.h data_structure/data_structure.h lexical_analysis/scanner.cpp lexical_analysis/scanner.h data_structure/token.cpp data_structure/token.h lexical_analysis/process_character.cpp lexical_analysis/process_character.h utility/automaton.cpp utility/automaton.h lexical_analysis/process_string.cpp lexical_analysis/process_string.h lexical_analysis/process_number.cpp lexical_analysis/process_number.h data_structure/errors.cpp data_structure/errors.h syntax_analysis/recursub.cpp syntax_analysis/recursub.h data_structure/quarternary.cpp data_structure/quarternary.h all_test/tables.cpp all_test/tables.h syntax_analysis/LL_1.cpp syntax_analysis/LL_1.h)

add_executable(CompileTest all_test/test.cpp)