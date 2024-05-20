//
// Created by @ruitianzhong on 5/13/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//

#include "execution/executor.h"
#include <cassert>
#include <db.pb.h>
#include <iostream>

#include "constant.h"

namespace xDB {
    void static createDatabase(const CreateStmt *stmt, rocksdb::DB *db) {
        std::string v;
        rocksdb::ReadOptions options;
        auto s = db->Get(rocksdb::ReadOptions(), Executor::MakeDBMetadataPrefix(stmt->name.schema), &v);
        if (s.IsNotFound()) {
            const DBMetadata db_metadata;
            std::string str;
            db_metadata.SerializeToString(&str);
            db->Put(rocksdb::WriteOptions(), DB_META_PREFIX + stmt->name.schema, str);
            std::cout << "Database " << stmt->name.schema << " is created" << std::endl;
            return;
        }
        std::cout << stmt->name.schema << " has already existed" << std::endl;
    }

    bool setUpMetaData(TableMetadata *metadata, const CreateStmt *stmt) {
        assert(stmt->list!=nullptr);
        std::unordered_set<std::string> s;
        bool hasPrimaryKey = false;

        for (const auto &def: *stmt->list) {
            assert(def->data_name!=nullptr);

            if (const auto ret = s.find(def->data_name); ret != s.end()) {
                std::cout << "duplcated variable definition `" << def->data_name << "`" << std::endl;
                return false;
            }
            s.insert(def->data_name);

            DBDefinition *dbd = metadata->add_definitions();
            dbd->set_nullable(true);
            if (def->constraints != nullptr) {
                // check constraint
                for (const auto constraint: *def->constraints) {
                    switch (constraint) {
                        case ConstraintNotNull:
                            dbd->set_nullable(false);
                            break;
                        case ConstraintPrimaryKey:
                            if (hasPrimaryKey) {
                                std::cout << "duplicate primary key" << std::endl;
                                return false;
                            }
                            dbd->set_nullable(false);
                            dbd->set_isprimary(true);
                            break;
                    }
                }
                if (dbd->isprimary()) {
                    hasPrimaryKey = true;
                }
            }

            CharDefinition *cd;
            dbd->set_name(def->data_name);
            switch (def->type) {
                case INTEGER:
                    dbd->set_type(DBDefinition::INTEGER);
                    break;
                case CHAR:
                    dbd->set_type(DBDefinition::CHAR);
                    cd = dynamic_cast<CharDefinition *>(def);
                    assert(cd!=nullptr);
                    dbd->set_charlen(cd->getLen());
                    break;
                case FLOAT:
                    dbd->set_type(DBDefinition::FLOAT);
                    break;
                default:
                    std::cout << "Unsupported type" << std::endl;
                    break;
            }
        }
        return true;
    }


    void Executor::createTable(const CreateStmt *stmt) const {
        assert(stmt->name.name!=nullptr);
        std::string cur = currentDB;
        if (stmt->name.schema != nullptr) {
            cur = stmt->name.schema;
        }
        if (cur.empty()) {
            std::cout << "No database selected" << std::endl;
            return;
        }
        const std::string key = MakeTableMetadataPrefix(cur, stmt->name.name);
        std::string value;

        auto s = db->Get(rocksdb::ReadOptions(), key, &value);
        if (s.ok()) {
            std::cout << "`" << cur << "." << stmt->name.name << "` has already existed" << std::endl;
            return;
        }
        TableMetadata data;

        if (s.IsNotFound() && setUpMetaData(&data, stmt)) {
            const auto ok = data.SerializeToString(&value);
            assert(ok);
            s = db->Put(rocksdb::WriteOptions(), key, value);
            assert(s.ok());
            std::cout << "`" << cur << "." << stmt->name.name << "` created" << std::endl;
        } else if (!s.ok() && !s.IsNotFound()) {
            std::cout << "Unexpected error: " << s.ToString() << std::endl;
        }
    }


    void Executor::executeCreateStmt(const CreateStmt *create_stmt) const {
        assert(create_stmt!=nullptr);
        switch (create_stmt->createType) {
            case CreateDatabase:
                std::cout << "creating db " << create_stmt->name.schema << std::endl;
                createDatabase(create_stmt, db);
                break;
            case CreateTable:
                createTable(create_stmt);
                break;
        }
    }
}
