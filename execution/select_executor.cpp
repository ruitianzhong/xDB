//
// Created by @ruitianzhong on 5/14/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
#include <utility>

#include "fort.hpp"
#include "execution/executor.h"

namespace xDB {
    ColumnFullName::ColumnFullName(std::string db_name, std::string table_name,
                                   std::string column_name): table_name(std::move(table_name)),
                                                             db_name(std::move(db_name)),
                                                             column_name(std::move(column_name)) {
    }

    std::string ColumnFullName::toString() const {
        return db_name + "." + table_name + "." + column_name;
    }


    static bool MakeTempRow(std::vector<TableFullName> &v, int index, const TempRow &row, // NOLINT(*-no-recursion)
                            std::vector<TempRow> &rows,
                            rocksdb::DB *db) {
        if (index == static_cast<int>(v.size())) {
            rows.push_back(row);
            return true;
        }

        std::vector<Row> table_rows;
        const auto it = db->NewIterator(rocksdb::ReadOptions());
        const auto fullname = v[index];
        const auto prefix = TABLE_ROW_PREFIX.append(fullname.table_schema).append(fullname.table_name);

        for (it->Seek(prefix); it->Valid(); it->Next()) {
            if (std::string str = it->key().data(); str.rfind(prefix, 0) != 0) {
                break;
            }

            auto value = it->value().ToString();
            Row curRow;
            if (!curRow.ParseFromString(value)) {
                std::cout << "Codec error" << std::endl;
                delete it;
                return false;
            }
            table_rows.push_back(curRow);
        }

        if (!it->status().ok()) {
            std::cout << it->status().ToString() << std::endl;
            delete it;
            return false;
        }

        delete it;

        for (const auto &table_row: table_rows) {
            TempRow nr = row;
            nr.addColumns(table_row);
            if (!MakeTempRow(v, index + 1, nr, rows, db)) {
                return false;
            }
        }

        return true;
    }

    void Executor::executeSelectStmt(const SelectStmt *select_stmt) const {
        // validate table: exist? duplicate?
        assert(select_stmt!=nullptr);
        auto table_names = select_stmt->table_names;
        assert(table_names!=nullptr);
        std::vector<TableFullName> v;
        std::unordered_set<std::string> duplicated_table;
        for (const auto table_name: *table_names) {
            assert(table_name.name!=nullptr);
            // check if table exist
            if (table_name.schema == nullptr && currentDB.empty()) {
                std::cout << "Can not infer table " << table_name.name << "'s database" << std::endl;
                return;
            }

            TableFullName full_name(table_name.schema == nullptr ? currentDB : table_name.schema, table_name.name);

            auto key = full_name.getTableMetaKey();

            std::string value;

            auto status = db->Get(rocksdb::ReadOptions(), key, &value);

            if (status.IsNotFound()) {
                std::cout << "Table `" << full_name.toString() << "` does not exist" << std::endl;
                return;
            }
            if (!status.ok()) {
                std::cout << __FUNCTION__ << " " << status.ToString() << std::endl;
                return;
            }
            auto id = full_name.toString();
            if (duplicated_table.find(id) != duplicated_table.end()) {
                std::cout << "duplicated table name please check again" << std::endl;
                return;
            }
            duplicated_table.insert(id);

            if (const auto ok = full_name.metadata.ParseFromString(value); !ok) {
                std::cout << __FUNCTION__ << " Unexpected error " << std::endl;
                return;
            }

            v.push_back(full_name);
        }

        std::unordered_map<std::string, int> column2index;
        auto fullname2index = std::unordered_map<ColumnFullName, int, ColumnFullNameHasher>();
        // full name to index mapping

        int count = 0;
        for (auto &i: v) {
            auto metadata = i.metadata;
            for (int j = 0; j < metadata.definitions_size(); j++) {
                const auto &def = metadata.definitions(j);
                column2index.insert({i.toStringWithColumn(def.name()), count});
                ColumnFullName full_name(i.table_schema, i.table_name, def.name());
                fullname2index.insert({full_name, count});
                count++;
            }
        }


        // check if field exist
        assert(select_stmt->column_names!=nullptr);
        std::vector<ColumnFullName> column_full_names;
        for (auto column_name: *select_stmt->column_names) {
            assert(column_name!=nullptr);
            assert(column_name->column_name!=nullptr);
            std::string colname = column_name->column_name;

            // handle wild card
            if (colname == "*") {
                for (const auto &table_full_name: v) {
                    for (const auto &def: table_full_name.metadata.definitions()) {
                        ColumnFullName column_full_name(table_full_name.table_schema, table_full_name.table_name,
                                                        def.name());
                        column_full_names.push_back(column_full_name);
                    }
                }
                continue;
            }

            if (column_name->schema == nullptr && currentDB.empty()) {
                std::cout << "No database selected for `" << column_name->name << "`" << std::endl;
                return;
            }

            std::string curDB = column_name->schema == nullptr ? currentDB : column_name->schema;
            bool found = false;


            if (column_name->name == nullptr) {
                for (auto &table: v) {
                    ColumnFullName column_full_name(curDB, table.table_name, colname);
                    auto columnKey = column_full_name.toString();

                    if (column2index.find(columnKey) != column2index.end()) {
                        if (found) {
                            std::cout << "ambiguous column name " << column_name->column_name << std::endl;
                            return;
                        }
                        found = true;
                        column_full_names.push_back(column_full_name);
                    }
                }
            } else {
                auto column_full_name = ColumnFullName(curDB, column_name->name, colname);
                if (found = column2index.find(column_full_name.toString()) !=
                            column2index.end(); found) {
                    column_full_names.push_back(column_full_name);
                }
            }

            if (!found) {
                std::cout << "Can not find column " << colname << std::endl;
                return;
            }
        }
        // staitic check for the where expression (best-effort)
        if (select_stmt->whereExp != nullptr) {
            auto context = ExecutionContext(TempRow(), fullname2index, v, currentDB, db);
            ExpChecker exp_checker(context);
            if (!visitExp(select_stmt->whereExp, &exp_checker)) {
                std::cout << "Please check the expression" << std::endl;
                return;
            }
        }
        // build the intermediate row here
        TempRow row;
        std::vector<TempRow> results;

        MakeTempRow(v, 0, row, results, db);

        fort::table table;
        table << fort::header;

        for (const auto &column_full_name: column_full_names) {
            table << column_full_name.toString();
        }
        table << fort::endr;

        int reuslt_cnt = 0;
        for (auto result: results) {
            if (select_stmt->whereExp != nullptr) {
                ExecutionContext context(result, fullname2index, v, currentDB, db);
                ExpEvaluator evaluator(context);

                bool ok = visitExp(select_stmt->whereExp, &evaluator);
                if (!ok) {
                    return;
                }
                ok = false;


                Value value = select_stmt->whereExp->getValue();
                switch (value.getType()) {
                    case ScalarInteger:
                        ok = value.getInteger() != 0;
                        break;
                    case ScalarChar:
                        std::cout << "[ Warning ] Char result :" << value.getChar() << std::endl;
                        break;

                    case ScalarNULL:
                        std::cout << "[ Warning ] NULL result" << std::endl;
                        break;
                    case ScalarInvalid:
                    default:
                        std::cout << "[Warning] unknown type" << std::endl;
                }
                select_stmt->whereExp->setValue(Value());

                if (!ok) { continue; }
            }

            reuslt_cnt++;
            for (const auto &col: column_full_names) {
                if (column2index.find(col.toString()) == column2index.end()) {
                    std::cout << "Can not find " << col.toString() << std::endl;
                    return;
                }
                int index = column2index[col.toString()];

                Column column = result.column(index);
                switch (column.type()) {
                    case Column::COLUMN_INT:
                        table << column.integer_num();
                        break;
                    case Column::COLUMN_CHAR:
                        table << column.str();
                        break;
                    case Column::COLUMN_NULL:
                        table << "NULL";
                        break;
                    default:
                        std::cout << "Some errors occur";
                        return;
                }
            }
            table << fort::endr;
        }
        if (reuslt_cnt > 0) {
            std::cout << "Result contains " << reuslt_cnt << " row(s)" << std::endl;
            std::cout << table.to_string() << std::endl;
        } else {
            std::cout << "Empty set" << std::endl;
        }
    }
}
