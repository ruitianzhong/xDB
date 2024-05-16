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

        std::unordered_map<ColumnFullName, int, ColumnFullNameHasher> m;

        buildColumnName2IndexMap(cur, tablename, m, metadata);
        std::vector<TableFullName> v;
        TableFullName t(cur, tablename);
        v.push_back(t);

        if (update_stmt->whereExp != nullptr) {
            ExecutionContext context(TempRow(), m, v, cur, db);
            ExpChecker checker(context);
            if (!visitExp(update_stmt->whereExp, &checker)) {
                std::cout << "Error on where expression" << std::endl;
                return;
            }
        }


        for (auto assign: *update_stmt->update_assigns) {
            ColumnName *c = assign->column_name;
            assert(c!=nullptr&&c->column_name!=nullptr);
            if (c->schema != nullptr && c->schema != cur) {
                std::cout << "No schema " << c->schema << std::endl;
                return;
            }

            if (c->name != nullptr && c->name != tablename) {
                std::cout << "No table " << c->name << std::endl;
                return;
            }

            ColumnFullName column_full_name(cur, tablename, c->column_name);

            if (m.find(column_full_name) == m.end()) {
                std::cout << "Can not find " << column_full_name.toString() << std::endl;
                return;
            }


            assert(assign->right!=nullptr);
            ExecutionContext context(TempRow(), m, v, cur, db);
            ExpChecker exp_checker(context);
            if (!visitExp(assign->right, &exp_checker)) {
                std::cout << "Error on set expression, please check it" << std::endl;
                return;
            }
        }
        std::vector<std::pair<TempRow, std::string> > rows;
        collectTableAllRows(rows, cur, tablename);

        //check assign here

        std::vector<std::pair<TempRow, std::string> > updatedRow;
        for (const auto &tempRow: rows) {
            //read all the row in rows here
            ExecutionContext context(tempRow.first, m, v, cur, db);
            if (update_stmt->whereExp != nullptr) {
                ExpEvaluator exp_evaluator(context);
                if (!visitExp(update_stmt->whereExp, &exp_evaluator)) {
                    std::cout << __FUNCTION__ << " unexpected error" << std::endl;
                    return;
                }
                Value value = update_stmt->whereExp->getValue();
                update_stmt->whereExp->setValue(Value());
                if (!value.ok()) continue;
            }
            auto row = tempRow.first;
            for (auto assign: *update_stmt->update_assigns) {
                ColumnFullName column_full_name(cur, tablename, assign->column_name->column_name);
                int index = m[column_full_name];

                ExpEvaluator evaluator(context);
                if (!visitExp(assign->right, &evaluator)) {
                    std::cout << "Error on expression evaluation" << std::endl;
                    return;
                }
                Value value = assign->right->getValue();
                assign->right->setValue(Value());
                DBDefinition::DefinitionType type = metadata.definitions(index).type();
                auto valuetype = value.getType();
                Column column = row.column(index);
                if (type == DBDefinition::CHAR && valuetype == ScalarChar) {
                    if (value.getChar().length() > metadata.definitions(index).charlen()) {
                        std::cout << value.getChar() << " is too long" << std::endl;
                        return;
                    }
                    column.set_type(Column::COLUMN_CHAR);
                    column.set_str(value.getChar());
                } else if (type == DBDefinition::INTEGER && valuetype == ScalarInteger) {
                    column.set_type(Column::COLUMN_INT);
                    column.set_integer_num(value.getInteger());
                    column.clear_str();
                } else if (value.getType() == ScalarNULL) {
                    column.set_type(Column::COLUMN_NULL);
                    column.clear_str();
                } else {
                    std::cout << __FUNCTION__ << " error " << std::endl;
                    return;
                }

                row.setColumn(index, column);
            }
            auto pair = std::pair(row, tempRow.second);
            updatedRow.push_back(pair);
        }
        if (updatedRow.empty()) {
            std::cout << "No row affected" << std::endl;
            return;
        }

        for (const auto &row: updatedRow) {
            Row stored_row;
            TempRow temp_row = row.first;
            for (int i = 0; i < metadata.definitions_size(); i++) {
                Column newColumn = temp_row.column(i);
                Column *cp = stored_row.add_columns();
                cp->CopyFrom(newColumn);
            }

            std::string serialized;

            if (!stored_row.SerializeToString(&serialized)) {
                std::cout << "Codec error in update executor" << std::endl;
                continue;
            }

            if (auto status = db->Put(rocksdb::WriteOptions(), row.second, serialized); !status.ok()) {
                std::cout << "Warning: " << status.ToString() << std::endl;
            }
        }

        std::cout << updatedRow.size() << " row(s) affected" << std::endl;
    }
}
