//
// Created by @ruitianzhong on 5/13/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//

#include "execution/executor.h"
#include <cassert>
#include <iostream>

#include "constant.h"

namespace xDB {
    void static createDatabase(CreateStmt *stmt, rocksdb::DB *db) {
        std::string v;
        rocksdb::ReadOptions options;
        auto s = db->Get(rocksdb::ReadOptions(), DB_META_PREFIX + stmt->name.schema, &v);
        if (s.IsNotFound()) {
            db->Put(rocksdb::WriteOptions(), DB_META_PREFIX + stmt->name.schema, "");
            std::cout << "Database " << stmt->name.schema << " is created" << std::endl;
            return;
        }
        std::cout << stmt->name.schema << " has already existed" << std::endl;
    }

    void static createTable(CreateStmt *stmt, rocksdb::DB *db) {
    }

    void Executor::executeCreateStmt(CreateStmt *create_stmt) {
        assert(create_stmt!=nullptr);
        switch (create_stmt->createType) {
            case CreateDatabase:
                std::cout << "creating db " << create_stmt->name.schema << std::endl;
                createDatabase(create_stmt, db);
                break;
            case CreateTable:
                std::cout << "Creating table " << create_stmt->name.name << std::endl;
                break;
        }
    }
}
