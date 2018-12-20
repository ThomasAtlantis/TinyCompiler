//
// Created by MAC on 2018/12/1.
//

#ifndef COMPILE_UTILITY_H
#define COMPILE_UTILITY_H

#include <windows.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <streambuf>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <map>
#include <set>

#define is_digit(ch) ((ch)>='0'&&(ch)<='9')
#define is_letter(ch) (((ch)>='a'&&(ch)<='z')||((ch)>='A'&&(ch)<='Z'))
#define is_blank(ch) ((ch)==' '||(ch)=='\n'||(ch)=='\t'||(ch)=='\r')
#define all(t) (t).begin(),(t).end()

using namespace std;

vector<string> split(const string& str, const string& sep = " ");
string join(vector<string> vec, const string& sep = " ");
ostream& operator<<(ostream& out, vector<string> vec);
ostream& operator<<(ostream& out, set<string> st);
#endif //COMPILE_UTILITY_H
