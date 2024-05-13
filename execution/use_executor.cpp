#include "execution/executor.h"
//
// Created by @ruitianzhong on 5/13/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
#include <cassert>
#include <iostream>

#include "constant.h"

namespace xDB {
    void Executor::executeUseStmt(const UseStmt *use_stmt) {
        assert(use_stmt!=nullptr);
        assert(use_stmt->name.schema!=nullptr);
        std::string value;
        auto s = db->Get(rocksdb::ReadOptions(),DB_META_PREFIX + use_stmt->name.schema, &value);
        if (s.ok()) {
            std::cout << "Use database `" << use_stmt->name.schema << "`." << std::endl;
            currentDB = use_stmt->name.schema;
        } else if (s.IsNotFound()) {
            std::cout << "Database `" << use_stmt->name.schema << "` does not exist." << std::endl;
        } else {
            std::cout << "Unexpected error " << s.ToString() << std::endl;
        }
    }
}
