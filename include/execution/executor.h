//
// Created by @ruitianzhong on 5/13/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//

#ifndef EXECUTOR_H
#define EXECUTOR_H
#include "db.pb.h"
#include <rocksdb/db.h>

#include "constant.h"
#include "sql/parser_result.h"
#include "sql/stmts.h"

namespace xDB {
    // forward declaration
    class TempRow {
    public:
        void addColumn(const Column &column);


        Column column(int index);

        void addColumns(const Row &row);

    private:
        std::vector<Column> columns_;
    };

    class ExecutionContext {
    public:
        ExecutionContext(TempRow temp_row, std::unordered_map<std::string, int> fullname2index);

        std::unordered_map<std::string, int> fullname2index() { return fullname2index_; }

        Column column(const int index) { return row.column(index); }

    private:
        std::unordered_map<std::string, int> fullname2index_;
        TempRow row;
    };

    struct ColumnFullName {
        std::string table_name, db_name, column_name;

        ColumnFullName(std::string db_name, std::string table_name,
                       std::string column_name);

        [[nodiscard]] std::string toString() const;
    };

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

        bool checkTable(TableName table_name, std::string &cur, std::string &table, TableMetadata &metadata) const;

        bool collectTableAllRows(std::vector<TempRow> &rows, const std::string &dbname,
                                 const std::string &tablename) const;

        static bool buildColumnName2IndexMap(std::unordered_map<std::string, int> m, const TableMetadata &metadata);

        bool evaluateExp(Exp *exp, ExpProcessor *processor);

        bool evaluateBetween(BetweenExpr *between_expr, ExpProcessor *processorr);

        static bool evaluateScalar(ScalarExp *exp, ExpProcessor *processor);

        bool evaluateBinary(BinaryExp *exp, ExpProcessor *processor);

        bool evaluateUnary(UnaryExp *exp, ExpProcessor *processor);


        std::string currentDB;
        rocksdb::DB *db;
    };

    class ExecutionResult {
    public:
        bool ok();
    };


    class ExpProcessor {
    public:
        explicit ExpProcessor(ExecutionContext context);

        bool process(BinaryExp *binary_exp);

        bool process(UnaryExp *unary_exp);

        bool process(BetweenExpr *between_expr);

        bool process(ScalarExp *scalar_exp);

    private:
        ExecutionContext context_;

        bool processNegate(UnaryExp *exp);

        bool processIsNull(UnaryExp *exp);

        bool processNot(UnaryExp *exp);

        bool processIsNotNull(UnaryExp *exp);

        bool processScalarName(ScalarExp *scalar_exp);
    };
}

#endif //EXECUTOR_H
