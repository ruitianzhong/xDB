//
// Created by @ruitianzhong on 5/13/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//

#ifndef EXECUTOR_H
#define EXECUTOR_H
#include "db.pb.h"
#include <rocksdb/db.h>

#include <utility>

#include "constant.h"
#include "sql/parser_result.h"
#include "sql/stmts.h"

namespace xDB {
    struct TableFullName {
        std::string table_name;
        std::string table_schema;
        TableMetadata metadata;

        TableFullName(std::string table_schema, std::string table_name): table_name(std::move(table_name)),
                                                                         table_schema(std::move(table_schema)) {
        }

        [[nodiscard]] std::string getTableMetaKey() const;

        [[nodiscard]] std::string toString() const {
            return table_schema + "." + table_name;
        }

        std::string toStringWithColumn(const std::string &column) const {
            return toString() + "." + column;
        }
    };

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
        ExecutionContext(TempRow temp_row, std::unordered_map<ColumnFullName, int, ColumnFullNameHasher> fullname2index,
                         std::vector<TableFullName> full_names, std::string curDB, rocksdb::DB *db);

        std::unordered_map<ColumnFullName, int, ColumnFullNameHasher> fullname2index() const { return fullname2index_; }

        Column column(const int index) { return row.column(index); }

        int table_size() const { return static_cast<int>(full_names_.size()); }

        TableFullName tables(const int idx) { return full_names_[idx]; }

        std::vector<TableFullName> tables() { return full_names_; }

        std::string curDB() const { return curDB_; }

        rocksdb::DB *getDB() const { return db_; }

    private:
        std::unordered_map<ColumnFullName, int, ColumnFullNameHasher> fullname2index_;
        TempRow row;
        std::vector<TableFullName> full_names_;
        std::string curDB_;
        rocksdb::DB *db_;
    };

    class ExecutionContextBuilder {
    public:
        ExecutionContextBuilder();

        ExecutionContextBuilder &add(TempRow row) {
            row_ = std::move(row);
            return *this;
        }

        ExecutionContextBuilder &add(std::vector<TableFullName> v) {
            v_ = std::move(v);
            return *this;
        }

        ExecutionContextBuilder &add(
            const std::unordered_map<ColumnFullName, int, ColumnFullNameHasher> &fullname2index) {
            fullname2index_ = fullname2index;
            return *this;
        }

        ExecutionContextBuilder &add(std::string curDB) {
            curDB_ = std::move(curDB);
            return *this;
        }

        ExecutionContextBuilder &add(rocksdb::DB *db) {
            db_ = db;
            return *this;
        }


        ExecutionContext build() const {
            return {row_, fullname2index_, v_, curDB_, db_};
        }

    private:
        TempRow row_;
        std::vector<TableFullName> v_;
        std::unordered_map<ColumnFullName, int, ColumnFullNameHasher> fullname2index_;
        std::string curDB_;
        rocksdb::DB *db_;
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

        static bool buildColumnName2IndexMap(const std::string &db, const std::string &table,
                                             std::unordered_map<ColumnFullName, int, ColumnFullNameHasher> &m,
                                             const TableMetadata &metadata);

        bool visitExp(Exp *exp, AbstractExpProcessor *processor) const;

        bool visitBetween(BetweenExpr *between_expr, AbstractExpProcessor *processor) const;

        bool visitScalar(ScalarExp *exp, AbstractExpProcessor *processor) const;

        bool visitBinary(BinaryExp *exp, AbstractExpProcessor *processor) const;

        bool visitUnary(UnaryExp *exp, AbstractExpProcessor *processor) const;


        std::string currentDB;
        rocksdb::DB *db;
    };

    class AbstractExpProcessor {
    public:
        explicit AbstractExpProcessor(ExecutionContext context);

        virtual bool process(BinaryExp *binary_exp);

        virtual bool process(UnaryExp *unary_exp);

        virtual bool process(BetweenExpr *between_expr);

        virtual bool process(ScalarExp *scalar_exp);

        virtual ~AbstractExpProcessor() = default;

    protected:
        ExecutionContext context_;

    private:
    };

    class ExpChecker final : public AbstractExpProcessor {
    public:
        explicit ExpChecker(ExecutionContext context): AbstractExpProcessor(std::move(context)) {
        }

        bool process(ScalarExp *scalar_exp) override;
    };

    class ExpEvaluator : public AbstractExpProcessor {
    public:
        explicit ExpEvaluator(ExecutionContext context): AbstractExpProcessor(std::move(context)) {
        }

        bool process(UnaryExp *unary_exp) override;

        bool process(BetweenExpr *between_expr) override;

        bool process(BinaryExp *binary_exp) override;

        bool process(ScalarExp *scalar_exp) override;

        bool processNegate(UnaryExp *exp);

        bool processIsNull(UnaryExp *exp);

        bool processNot(UnaryExp *exp);

        bool processIsNotNull(UnaryExp *exp);

        bool processScalarName(ScalarExp *scalar_exp);
    };
}

#endif //EXECUTOR_H
