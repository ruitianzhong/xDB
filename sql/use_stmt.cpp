#include "sql/use_stmt.h"
#include "sql/table.h"
//
// Created by @ruitianzhong on 5/11/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//


namespace xDB {
    UseStmt::UseStmt(const TableName name_): SQLStmt(xSQL_USE), name(name_) {
    }

    UseStmt::~UseStmt() = default;
}
