//
// Created by rtzhong on 5/7/24.
//
#include <iostream>

#include "sql/parser_result.h"


int wrapped_parse(const char *text, xDB::ParserResult *result);

std::string s;

void read_loop();

int main(int argc, char *argv[]) {
    read_loop();
    return 0;
}

void read_loop() {
    while (true) {
        std::getline(std::cin, s);
        auto result = new xDB::ParserResult();
        wrapped_parse(s.c_str(), result);
    }
}
