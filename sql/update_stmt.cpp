//
// Created by @ruitianzhong on 5/12/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//

#include "sql/update_stmt.h"

#include "sql/exp.h"

namespace xDB {
    UpdateAssign::UpdateAssign(ColumnName *column_name_, Exp *right_) : column_name(column_name_), right(right_) {
    }

    UpdateStmt::UpdateStmt(TableName table_name_, std::vector<UpdateAssign *> *list_,
                           Exp *whereExp_): SQLStmt(xSQL_UPDATE),
                                            table_name(table_name_), update_assigns(list_), whereExp(whereExp_) {
    }

    UpdateStmt::~UpdateStmt() = default;
}
