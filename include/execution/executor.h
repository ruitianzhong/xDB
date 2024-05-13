//
// Created by @ruitianzhong on 5/13/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//

#ifndef EXECUTOR_H
#define EXECUTOR_H
#include <rocksdb/db.h>

#include "sql/parser_result.h"
#include "sql/stmts.h"

namespace xDB {
    class Executor {
    public:
        Executor();

        bool execute(ParserResult *result);

        bool dispatch(SQLStmt *stmt);

        void shutdown() const;

        void executeCreateStmt(CreateStmt *create_stmt);

        void executeUseStmt(UseStmt *use_stmt);

        bool init();

    private:
        void createTable(CreateStmt *stmt);

        std::string currentDB;
        rocksdb::DB *db;
    };

    class ExecutionResult {
    public:
        bool ok();
    };
}

#endif //EXECUTOR_H
