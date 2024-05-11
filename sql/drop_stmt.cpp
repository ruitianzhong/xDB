//
// Created by @ruitianzhong on 5/11/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
#include "sql/drop_stmt.h"
#include "sql/stmt.h"

namespace xDB {
    DropStmt::DropStmt(const DropType type): SQLStmt(xSQL_DROP), type(type) {
    }
}
