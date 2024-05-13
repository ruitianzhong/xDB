//
// Created by rtzhong on 5/7/24.
//
#include <iostream>

#include "sql/parser_result.h"
#include "rocksdb/db.h"
#include <cassert>
#include "fort.hpp"
#include "common/codec/db.pb.h"
#include "execution/executor.h"

int wrapped_parse(const char *text, xDB::ParserResult *result);

std::string s;

void read_loop();


void read_loop() {
    xDB::Executor executor;
    executor.init();
    while (true) {
        std::getline(std::cin, s);
        auto result = new xDB::ParserResult();
        wrapped_parse(s.c_str(), result);
        if (!executor.execute(result)) {
            break;
        }
    }
    executor.shutdown();
}


void tryRocksDB() {
}

void fort_test() {
    fort::char_table table;
    table << fort::header
            << "N" << "Driver" << "Time" << "Avg Speed" << fort::endr
            << "1" << "Ricciardo" << "1:25.945" << "47.362" << fort::endr
            << "2" << "Hamilton" << "1:26.373" << "35.02" << fort::endr
            << "3" << "Verstappen" << "1:26.469" << "29.22" << fort::endr;

    std::cout << table.to_string() << std::endl;
}

void protobuf_test() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    auto person = xDB::Person();

    person.set_id(42);
    person.set_name("hello");
    std::string output;
    bool ok = person.SerializeToString(&output);
    assert(ok);
    auto decode = xDB::Person();
    ok = decode.ParseFromString(output);
    assert(ok);
    std::cout << "protobuf test ended";
    std::cout << decode.name() << std::endl;
}

int main(int argc, char **argv) {
    fort_test();
    tryRocksDB();
    protobuf_test();
    read_loop();
    return 0;
}
