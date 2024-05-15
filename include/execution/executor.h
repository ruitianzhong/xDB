//
// Created by @ruitianzhong on 5/13/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//

#ifndef EXECUTOR_H
#define EXECUTOR_H
#include <db.pb.h>
#include <rocksdb/db.h>

#include "constant.h"
#include "sql/parser_result.h"
#include "sql/stmts.h"

namespace xDB {
    // forward declaration
    class TempRow;

    class Executor {
    public:
        Executor();

        bool execute(ParserResult *result);

        bool dispatch(SQLStmt *stmt);

        void shutdown() const;

        void executeCreateStmt(const CreateStmt *create_stmt) const;

        void executeUseStmt(const UseStmt *use_stmt);

        void executeShowStmt(const ShowStmt *show_stmt) const;

        void executeInsertStmt(const InsertStmt *insert_stmt) const;

        void executeSelectStmt(const SelectStmt *select_stmt) const;

        void executeUpdateStmt(const UpdateStmt *update_stmt) const;

        void executeDeleteStmt(const DeleteStmt *delete_stmt) const;

        void executeDropStmt(const DropStmt *drop_stmt);

        bool init();

        static std::string MakeTableMetadataPrefix(const std::string &dbname, const std::string &table_name) {
            return TABLE_META_PREFIX + dbname + table_name;
        }

        // ??
        static std::string MakeDBMetadataPrefix(const std::string &dbname) {
            return DB_META_PREFIX + dbname;
        }

    private:
        static bool handleEachParam(const TableMetadata &metadata, std::vector<Parameter *> *param,
                                    const InsertStmt *stmt,
                                    Row &row);

        void createTable(const CreateStmt *stmt) const;

        void showTables() const;

        void showDatabases() const;

        void dropTable(const DropStmt *drop_stmt) const;

        void dropDatabase(const DropStmt *drop_stmt);

        bool checkTable(TableName table_name, std::string &cur, std::string &table) const;

        bool collectTableAllRows(std::vector<TempRow> &rows, const std::string &dbname,
                                 const std::string &tablename) const;

        std::string currentDB;
        rocksdb::DB *db;
    };

    class ExecutionResult {
    public:
        bool ok();
    };

    class TempRow {
    public:
        void addColumn(const Column &column);


        Column column(int index);

        void addColumns(const Row &row);

    private:
        std::vector<Column> columns_;
    };
}

#endif //EXECUTOR_H
