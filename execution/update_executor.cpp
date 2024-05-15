#include "execution/executor.h"
//
// Created by @ruitianzhong on 5/14/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
namespace xDB {
    void Executor::executeUpdateStmt(const UpdateStmt *update_stmt) const {
        assert(update_stmt!=nullptr);
        std::string cur, tablename;
        if (!checkTable(update_stmt->table_name, cur, tablename)) {
            return;
        }

        std::vector<TempRow> rows;
        collectTableAllRows(rows, cur, tablename);
    }
}
