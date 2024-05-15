#include "execution/executor.h"
//
// Created by @ruitianzhong on 5/14/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
namespace xDB {
    void Executor::executeUpdateStmt(const UpdateStmt *update_stmt) const {
        assert(update_stmt!=nullptr);
        assert(update_stmt->update_assigns!=nullptr);
        std::string cur, tablename;
        TableMetadata metadata;
        if (!checkTable(update_stmt->table_name, cur, tablename, metadata)) {
            return;
        }


        std::vector<TempRow> rows;
        collectTableAllRows(rows, cur, tablename);
        std::unordered_map<std::string, int> m;
        buildColumnName2IndexMap(m, metadata);

        //check assign here
        for (auto assign: *update_stmt->update_assigns) {
            assert(assign!=nullptr&&assign->column_name!=nullptr&&assign->right!=nullptr);
            std::string name = assign->column_name->column_name;
            if (m.find(name) == m.end()) {
                std::cout << "No column " << name << std::endl;
                return;
            }
        }
        std::vector<Row> updatedRow;
        for (auto tempRow: rows) {
        }
    }
}
