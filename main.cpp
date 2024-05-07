//
// Created by rtzhong on 5/7/24.
//
#include <iostream>
#include "binder/binder.h"

int fake();


int main(int argc, char *argv[]) {
    std::cout << "hello world" << std::endl;
    hello();
    return id(42) + fake();
}
