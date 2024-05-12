//
// Created by @ruitianzhong on 5/11/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
#include "sql/create_stmt.h"

#include "sql/table.h"
#include <cassert>

namespace xDB {
    // may be ambiguous for TableName here.
    CreateStmt::CreateStmt(TableName name_) : SQLStmt(xSQL_CREATE), name(name_),
                                              createType(CreateDatabase), list(nullptr) {
        assert(name.schema!=nullptr&& name.name==nullptr);
    }

    CreateStmt::CreateStmt(TableName name_, std::vector<DataDefinition *> *list_) : SQLStmt(xSQL_CREATE),
        name(name_), createType(CreateTable),
        list(list_) {
        assert(name.name!=nullptr);
    }

    CreateStmt::~CreateStmt() = default;
}
