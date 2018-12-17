//
// Created by MAC on 2018/12/1.
//

#include "utility.h"

vector<string> split(const string& str, const string& sep) {
    vector<string> vec;
    auto *c_str = new char[str.length()];
    strcpy(c_str, str.c_str());
    char *p = strtok(c_str, sep.c_str());
    while (p) {
        vec.emplace_back(string(p));
        p = strtok(nullptr, sep.c_str());
    }
//    delete [] c_str; // Del it for debugging
    return vec;
}

string join(vector<string> vec, const string& sep) {
    if (vec.size() == 1) return vec.front();
    string result = vec.front();
    for_each(vec.begin() + 1, vec.end(), [&](string str){
        result = result + sep + str;
    });
    return result;
}

ostream& operator<<(ostream& out, vector<string> vec) {
    for (const auto &str: vec) out << str << " ";
    return out;
}

ostream& operator<<(ostream& out, set<string> st) {
    for (const auto &str: st) out << str << " ";
    return out;
}