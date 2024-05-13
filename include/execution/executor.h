//
// Created by @ruitianzhong on 5/13/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//

#ifndef EXECUTOR_H
#define EXECUTOR_H
#include <rocksdb/db.h>

#include "constant.h"
#include "sql/parser_result.h"
#include "sql/stmts.h"

namespace xDB {
    class Executor {
    public:
        Executor();

        bool execute(ParserResult *result);

        bool dispatch(SQLStmt *stmt);

        void shutdown() const;

        void executeCreateStmt(CreateStmt *create_stmt) const;

        void executeUseStmt(const UseStmt *use_stmt);

        void executeShowStmt(const ShowStmt *show_stmt) const;

        void executeInsertStmt(const InsertStmt *insert_stmt) const;

        bool init();

        static std::string MakeTableMetadataPrefix(const std::string &dbname, const std::string &table_name) {
            return TABLE_META_PREFIX + dbname + table_name;
        }

        // ??
        static std::string MakeDBMetadataPrefix(const std::string &dbname) {
            return DB_META_PREFIX + dbname;
        }

    private:
        void createTable(CreateStmt *stmt) const;

        void showTables() const;

        void showDatabases() const;

        std::string currentDB;
        rocksdb::DB *db;
    };

    class ExecutionResult {
    public:
        bool ok();
    };
}

#endif //EXECUTOR_H
