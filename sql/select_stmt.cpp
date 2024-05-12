//
// Created by @ruitianzhong on 5/12/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
#include "sql/select_stmt.h"
#include <vector>

#include "sql/table.h"
#include "sql/exp.h"

namespace xDB {
    SelectStmt::SelectStmt(std::vector<ColumnName *> *column_names_, std::vector<TableName> *table_names_,
                           Exp *whereExp_): SQLStmt(xSQL_SELECT), column_names(column_names_),
                                            table_names(table_names_), whereExp(whereExp_) {
    }
}
