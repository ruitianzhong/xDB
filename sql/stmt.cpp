//
// Created by @ruitianzhong on 5/11/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//

#include "sql/stmt.h"

namespace xDB {
    SQLStmt::SQLStmt(const SQLStmtType type) : type_(type) {
    }

    SQLStmtType SQLStmt::type() const {
        return type_;
    }

    SQLStmt::~SQLStmt() = default;
}
