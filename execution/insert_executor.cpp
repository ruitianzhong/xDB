#include <db.pb.h>
#include <iostream>

#include "execution/executor.h"
//
// Created by @ruitianzhong on 5/13/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
namespace xDB {
    void Executor::executeInsertStmt(const InsertStmt *insert_stmt) const {
        assert(insert_stmt!=nullptr);
        assert(insert_stmt->parameters!=nullptr);
        assert(insert_stmt->table_name.name!=nullptr);
        std::string value, cur = currentDB;
        if (insert_stmt->table_name.schema != nullptr) {
            cur = insert_stmt->table_name.schema;
        }
        if (cur.empty()) {
            std::cout << "No database selected" << std::endl;
            return;
        }
        const std::string metaKey = MakeTableMetadataPrefix(cur, insert_stmt->table_name.name);
        auto s = db->Get(rocksdb::ReadOptions(), metaKey, &value);
        if (!s.ok()) {
            if (s.IsNotFound()) {
                std::cout << "Table `" << cur << "." << insert_stmt->table_name.name << "` does not exist" << std::endl;
            }
            return;
        }
        TableMetadata metadata;
        if (const auto ok = metadata.ParseFromString(value); !ok) {
            std::cout << "Codec error" << std::endl;
            return;
        }
        rocksdb::WriteBatch batch;
        for (auto param: *insert_stmt->parameters) {
            unsigned long nextid = metadata.nextid();
            Row row;
            if (!handleEachParam(metadata, param, insert_stmt, row)) {
                return;
            }
            auto sid = std::to_string(nextid);

            std::string key = TABLE_ROW_PREFIX.append(cur).append(insert_stmt->table_name.name).append(sid);
            if (!row.SerializeToString(&value)) {
                std::cout << "Codec error" << std::endl;
                return;
            }
            batch.Put(key, value);
            metadata.set_nextid(metadata.nextid() + 1);
        }
        if (!metadata.SerializeToString(&value)) {
            return;
        }
        batch.Put(metaKey, value);
        auto status = db->Write(rocksdb::WriteOptions(), &batch);
        if (!status.ok()) {
            std::cout << __FUNCTION__ << " " << status.ToString() << std::endl;
        } else {
            std::cout << "Insert Ok" << std::endl;
        }
    }

    static bool checkAndMakeRow(const DBDefinition &def, const Parameter *parameter, Row &row) {
        Column *column;

        assert(parameter!=nullptr);
        if (def.type() == DBDefinition::INTEGER && parameter->getType() == INTEGER) {
            column = row.add_columns();
            column->set_type(Column::COLUMN_INT);
            column->set_integer_num(parameter->getInt());
        } else if (def.type() == DBDefinition::CHAR && parameter->getType() == CHAR) {
            const auto str = parameter->getChar();
            assert(str!=nullptr);
            if (strlen(str) > static_cast<size_t>(def.charlen())) {
                std::cout << "Char too long" << std::endl;
                return false;
            }
            column = row.add_columns();
            column->set_type(Column::COLUMN_CHAR);
            column->set_str(str);
        } else if (def.type() == DBDefinition::FLOAT && parameter->getType() == FLOAT) {
            column = row.add_columns();
            column->set_type(Column::COLUMN_FLOAT);
            column->set_float_num(parameter->getDouble());
        } else if (def.type() == DBDefinition::FLOAT && parameter->getType() == INTEGER) {
            column = row.add_columns();
            column->set_type(Column::COLUMN_FLOAT);
            column->set_float_num(parameter->getInt());
        } else if (parameter->getType() == DataTypeNULL) {
            if (!def.nullable()) {
                std::cout << "column " << def.name() << " can not be null" << std::endl;
                return false;
            }
            column = row.add_columns();
            column->set_type(Column::COLUMN_NULL);
        } else {
            std::cout << "Data type mismatched" << std::endl;
            return false;
        }
        return true;
    }

    bool Executor::handleEachParam(const TableMetadata &metadata, std::vector<Parameter *> *param,
                                   const InsertStmt *stmt,
                                   Row &row) {
        assert(param!=nullptr);
        // check if column count matched
        if (stmt->opt_column_names == nullptr && metadata.definitions_size() != static_cast<int>(param->size())) {
            std::cout << "Column count doesn't match value count at row 1" << std::endl;
            return false;
        }
        if (metadata.definitions_size() < static_cast<int>(param->size())) {
            std::cout << "Too much argument" << std::endl;
            return false;
        }
        // check optional column name
        // where is the parameter?
        std::unordered_map<std::string, int> nameMap;
        for (int i = 0; i < metadata.definitions_size(); i++) {
            const auto &def = metadata.definitions(i);
            nameMap.insert({def.name(), -1});
        }
        if (stmt->opt_column_names != nullptr) {
            for (int i = 0; i < static_cast<int>(stmt->opt_column_names->size()); i++) {
                const auto name = stmt->opt_column_names->at(i);
                assert(name!=nullptr && name->column_name!=nullptr);
                if (nameMap.find(name->column_name) == nameMap.end()) {
                    std::cout << "Unknown column name" << std::endl;
                    return false;
                }
                nameMap[name->column_name] = i;
            }
        }


        std::unordered_map<std::string, Parameter *> checkMap;
        for (int i = 0; i < metadata.definitions_size(); i++) {
            if (stmt->opt_column_names == nullptr) {
                checkMap.insert({metadata.definitions(i).name(), param->at(i)});
            } else {
                const int index = nameMap[metadata.definitions(i).name()];
                if (index == -1) {
                    continue;
                }

                checkMap.insert({
                    metadata.definitions(i).name(),
                    param->at(index)
                });
            }
        }

        for (int i = 0; i < metadata.definitions_size(); i++) {
            const auto &def = metadata.definitions(i);
            if (checkMap.find(def.name()) == checkMap.end()) {
                Column *column = row.add_columns();
                column->set_type(Column::COLUMN_NULL);
            } else {
                if (const auto parameter = checkMap[def.name()]; !checkAndMakeRow(def, parameter, row)) {
                    return false;
                }
            }
        }
        return true;
    }
}
