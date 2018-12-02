cmake_minimum_required(VERSION 3.12)
project(Compile)

set(CMAKE_CXX_STANDARD 11)

add_executable(Compile main.cpp data_structure/grammar.cpp data_structure/grammar.h utility/utility.cpp utility/utility.h data_structure/tables.cpp data_structure/tables.h data_structure/data_structure.h lexical_analysis/scanner.cpp lexical_analysis/scanner.h)