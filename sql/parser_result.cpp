//
// Created by @ruitianzhong on 5/12/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
#include "sql/parser_result.h"


namespace xDB {
    void ParserResult::addStatement(SQLStmt *stmt) {
        statements.push_back(stmt);
    }
}
