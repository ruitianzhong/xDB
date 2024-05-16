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
        // check expression
        std::unordered_map<ColumnFullName, int, ColumnFullNameHasher> m;

        std::vector<TableFullName> v;
        TableFullName table_full_name(cur, table);
        v.push_back(table_full_name);

        buildColumnName2IndexMap(cur, table, m, metadata);
        if (delete_stmt->whereExp != nullptr) {
            auto context = ExecutionContext(TempRow(), m, v, cur, db);
            auto expChecker = ExpChecker(context);

            if (!visitExp(delete_stmt->whereExp, &expChecker)) {
                std::cout << "[ Warning ] " << "static check fails on where expression" << std::endl;
                return;
            }
        }

        std::vector<std::pair<TempRow, std::string> > rows;
        collectTableAllRows(rows, cur, table);
        std::vector<std::pair<TempRow, std::string> > delete_list;
        for (const auto &result: rows) {
            if (delete_stmt->whereExp != nullptr) {
                ExecutionContext context(result.first, m, v, cur, db);
                ExpEvaluator evaluator(context);

                if (!visitExp(delete_stmt->whereExp, &evaluator)) {
                    std::cout << "[ Warning ] Delete where expression" << std::endl;
                    continue;
                }
                Value value = delete_stmt->whereExp->getValue();
                // reset
                delete_stmt->whereExp->setValue(Value());

                if (!value.ok()) {
                    continue;
                }
            }
            delete_list.push_back(result);
        }

        if (delete_list.empty()) {
            std::cout << "Empty set" << std::endl;
            return;
        }

        for (const auto &pair: delete_list) {
            auto status = db->Delete(rocksdb::WriteOptions(), pair.second);
            if (!status.ok()) {
                std::cout << "[ Warn ] " << status.ToString() << std::endl;
            }
        }

        std::cout << delete_list.size() << " rows affected" << std::endl;
    }
}
