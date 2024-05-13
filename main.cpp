//
// Created by rtzhong on 5/7/24.
//
#include <iostream>

#include "sql/parser_result.h"
#include "rocksdb/db.h"
#include <cassert>
#include "fort.hpp"

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
    if (status.ok()) status = db->Put(rocksdb::WriteOptions(), "1", "1");
    std::string value;
    if (status.ok()) status = db->Get(rocksdb::ReadOptions(), "2", &value);
    std::cout << status.ok();
    // assert(status.ok());

    status = db->Delete(rocksdb::WriteOptions(), "1");
    assert(status.ok());

    rocksdb::WaitForCompactOptions opt = rocksdb::WaitForCompactOptions();
    opt.close_db = true;
    status = db->WaitForCompact(opt);
    std::cout << status.ok();
    delete db;
}

void fort_test() {
    fort::char_table table;
    table << fort::header
            << "N" << "Driver" << "Time" << "Avg Speed" << fort::endr
            << "1" << "Ricciardo" << "1:25.945" << "47.362" << fort::endr
            << "2" << "Hamilton" << "1:26.373" << "35.02" << fort::endr
            << "3" << "Verstappen" << "1:26.469" << "29.78" << fort::endr;

    std::cout << table.to_string() << std::endl;
}

int main(int argc, char **argv) {
    fort_test();
    tryRocksDB();
    read_loop();
    return 0;
}
