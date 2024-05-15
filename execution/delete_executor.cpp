//
// Created by @ruitianzhong on 5/14/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//

#include "execution/executor.h"

namespace xDB {
    void Executor::executeDeleteStmt(const DeleteStmt *delete_stmt) const {
        assert(delete_stmt!=nullptr);

        std::string cur;
        std::string table;
        TableMetadata metadata;
        // check if the table exist?
        if (!checkTable(delete_stmt->table_name, cur, table, metadata)) {
            return;
        }

        std::vector<TempRow> rows;
        collectTableAllRows(rows, cur, table);
        std::unordered_map<std::string, int> m;
        buildColumnName2IndexMap(m, metadata);
    }
}
