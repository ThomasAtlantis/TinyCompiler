//
// Created by MAC on 2018/12/13.
//

#include <iostream>

int a = 0;
int b = a + 1;

int function() {
    int c = 0;
    int b = function();
    return 0;
}

int main() {
    std::cout << function();
    std::cout << function();
    std::cout << function();
    std::cout << function();
    std::cout << a << ", " << b << std::endl;
    int a.2;
    return 0;
}