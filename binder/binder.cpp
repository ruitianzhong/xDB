//
// Created by rtzhong on 5/7/24.
//

#include <iostream>
#include "binder/binder.h"

void hello() {
    std::cout << "hello world" << std::endl;
}

int id(int x) {
    return x;
}
