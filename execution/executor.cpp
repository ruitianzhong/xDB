//
// Created by @ruitianzhong on 5/13/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//

#include "execution/executor.h"

#include <iostream>


#include "sql/parser_result.h"

namespace xDB {
    Executor::Executor() = default;


    bool Executor::execute(ParserResult *results) {
        for (auto &result: *results->getStatements()) {
            if (!dispatch(result)) {
                return false;
            }
        }
        return true;
    }

    bool Executor::dispatch(SQLStmt *stmt) {
        switch (stmt->type()) {
            case xSQL_USE:
                executeUseStmt(dynamic_cast<UseStmt *>(stmt));
                break;
            case xSQL_DROP:
                break;
            case xSQL_SHOW:
                executeShowStmt(dynamic_cast<ShowStmt *>(stmt));
                break;
            case xSQL_DELETE:
                break;
            case xSQL_INSERT:
                executeInsertStmt(dynamic_cast<InsertStmt *>(stmt));
                break;
            case xSQL_CREATE:
                executeCreateStmt(dynamic_cast<CreateStmt *>(stmt));
                break;
            case xSQL_EXIT:
                return false;
            case xSQL_SELECT:
                executeSelectStmt(dynamic_cast<SelectStmt *>(stmt));
                break;
            default:
                std::cout << "Unknown SQL statement type" << std::endl;
                break;
        }
        return true;
    }

    void Executor::shutdown() const {
        rocksdb::WaitForCompactOptions opt = rocksdb::WaitForCompactOptions();
        opt.close_db = true;
        auto s = db->WaitForCompact(opt);
        assert(s.ok());
        delete db;
    }

    bool ExecutionResult::ok() {
        return true;
    }

    bool Executor::init() {
        rocksdb::Options options;
        options.create_if_missing = true;
        rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/testdb", &db);
        assert(status.ok());
        return true;
        // if (status.ok()) status = db->Put(rocksdb::WriteOptions(), "1", "1");
        // std::string value;
        // if (status.ok()) status = db->Get(rocksdb::ReadOptions(), "2", &value);
        // std::cout << status.ok();
        // assert(status.ok());

        // status = db->Delete(rocksdb::WriteOptions(), "1");
        // assert(status.ok());
    }

    Column TempRow::column(int index) {
        return columns_[index];
    }

    void TempRow::addColumn(const Column &column) {
        columns_.push_back(column);
    }

    void TempRow::addColumns(const Row &row) {
        for (const auto &col: row.columns()) {
            columns_.push_back(col);
        }
    }
}
