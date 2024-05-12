//
// Created by rtzhong on 5/7/24.
//
#include <iostream>

#include "sql/parser_result.h"
#include "rocksdb/db.h"
#include <cassert>

int wrapped_parse(const char *text, xDB::ParserResult *result);

std::string s;

void read_loop();


void read_loop() {
    while (true) {
        std::getline(std::cin, s);
        auto result = new xDB::ParserResult();
        wrapped_parse(s.c_str(), result);
    }
}


void tryRocksDB() {
    rocksdb::DB *db;
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/testdb", &db);
    assert(status.ok());
    rocksdb::WaitForCompactOptions opt = rocksdb::WaitForCompactOptions();
    opt.close_db = true;
    status = db->WaitForCompact(opt);
    std::cout << status.ok();
    delete db;
}


int main(int argc, char **argv) {
    tryRocksDB();
    read_loop();
    return 0;
}
