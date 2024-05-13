#include "execution/executor.h"
//
// Created by @ruitianzhong on 5/13/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
#include <cassert>
#include <iostream>

#include "fort.hpp"

namespace xDB {
    void Executor::executeShowStmt(const ShowStmt *show_stmt) const{
        assert(show_stmt!=nullptr);
        switch (show_stmt->getShowType()) {
            case ShowDatabases:
                showDatabases();
                break;
            case ShowTables:
                showTables();
                break;
        }
    }

    void Executor::showTables() const {
        if (currentDB.empty()) {
            std::cout << "No database selected" << std::endl;
            return;
        }
        const std::string prefix = MakeTableMetadataPrefix(currentDB, "");
        fort::char_table table;
        table << fort::header << "Tables" << fort::endr;
        const auto it = db->NewIterator(rocksdb::ReadOptions());
        for (it->Seek(prefix); it->Valid(); it->Next()) {
            std::string_view sv = it->key().data();
            if (sv.rfind(prefix, 0) != 0) {
                break;
            }
            sv.remove_prefix(prefix.size());
            table << sv << fort::endr;
        }
        if (!it->status().ok()) {
            std::cout << __FUNCTION__ << " " << it->status().ToString() << std::endl;
        }
        std::cout << table.to_string() << std::endl;
        delete it;
    }

    void Executor::showDatabases() const {
        // Iterator should be deleted after using it
        const auto it = db->NewIterator(rocksdb::ReadOptions());
        fort::char_table table;
        table << fort::header
                << "Databases" << fort::endr;

        for (it->Seek(DB_META_PREFIX); it->Valid(); it->Next()) {
            auto s = it->key();

            std::string_view sv = s.data();
            if (sv.rfind(DB_META_PREFIX, 0) != 0) {
                break;
            }
            sv.remove_prefix(DB_META_PREFIX.length());
            table << sv << fort::endr;
        }

        std::cout << table.to_string() << std::endl;
        if (!it->status().ok()) {
            std::cout << __FUNCTION__ << ":" << it->status().ToString() << std::endl;
        }
        delete it;
    }
}
