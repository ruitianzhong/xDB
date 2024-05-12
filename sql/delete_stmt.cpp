//
// Created by @ruitianzhong on 5/12/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//

#include "sql/delete_stmt.h"

namespace xDB {
    DeleteStmt::~DeleteStmt() = default;

    DeleteStmt::DeleteStmt(TableName table_name_, Exp *whereExp_) : SQLStmt(xSQL_DELETE), table_name(table_name_),
                                                                    whereExp(whereExp_) {
    }
}
