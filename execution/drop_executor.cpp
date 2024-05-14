#include "execution/executor.h"
//
// Created by @ruitianzhong on 5/14/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
namespace xDB {
    static bool deletePrefix(const std::string &prefix, rocksdb::DB *db, rocksdb::Iterator *it) {
        for (it->Seek(prefix); it->Valid(); it->Next()) {
            auto key = it->key().ToString();
            if (key.rfind(prefix, 0) != 0) {
                break;
            }
            db->Delete(rocksdb::WriteOptions(), key);
        }
        if (!it->status().ok()) {
            std::cout << "[ Warning :: delete] " << it->status().ToString() << std::endl;
            return false;
        }
        return true;
    }

    void Executor::dropTable(const DropStmt *drop_stmt) const {
        assert(drop_stmt->name.name!=nullptr);

        if (drop_stmt->name.schema == nullptr && currentDB.empty()) {
            std::cout << "No database selected" << std::endl;
            return;
        }

        std::string cur = drop_stmt->name.schema == nullptr ? currentDB : drop_stmt->name.schema;
        std::string table_name = drop_stmt->name.name;

        const auto it = db->NewIterator(rocksdb::ReadOptions());
        const std::string table_meta_prefix = TABLE_META_PREFIX + cur + table_name;
        const std::string table_row_prefix = TABLE_ROW_PREFIX + cur + table_name;

        deletePrefix(table_meta_prefix, db, it);
        deletePrefix(table_row_prefix, db, it);

        delete it;
    }

    void Executor::dropDatabase(const DropStmt *drop_stmt) {
        assert(drop_stmt->name.name==nullptr&& drop_stmt->name.schema!=nullptr);
        const std::string dbMetaKey = MakeDBMetadataPrefix(drop_stmt->name.schema);
        const std::string dbname = drop_stmt->name.schema;

        if (const auto status = db->Delete(rocksdb::WriteOptions(), dbMetaKey); !status.ok()) {
            std::cout << "[ Warning :: delete] " << status.ToString() << std::endl;
        }
        const std::string tableMetaPrefix = TABLE_META_PREFIX + dbname;
        const auto it = db->NewIterator(rocksdb::ReadOptions());
        deletePrefix(tableMetaPrefix, db, it);
        std::string tableRowPrefix = TABLE_ROW_PREFIX + dbname;
        deletePrefix(tableRowPrefix, db, it);
        delete it;

        // switch to empty
        std::string s;
        currentDB = "";
        std::cout << "Drop database " << dbname << std::endl;
    }


    void Executor::executeDropStmt(const DropStmt *drop_stmt) {
        assert(drop_stmt!=nullptr);
        switch (drop_stmt->type) {
            case DropDatabase:
                dropDatabase(drop_stmt);
                break;
            case DropTable:
                dropTable(drop_stmt);
                break;
        }
    }
}
