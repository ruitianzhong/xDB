//
// Created by rtzhong on 5/10/24.
//

#ifndef SELECT_STMT_H
#define SELECT_STMT_H
#include <vector>

#include "exp.h"
#include "stmt.h"
#include "table.h"

namespace xDB {
    class SelectStmt : public SQLStmt {
    public:
        SelectStmt(std::vector<ColumnName *> *column_names_, std::vector<TableName> *table_names_, Exp *whereExp_);

        std::vector<ColumnName *> *column_names;
        std::vector<TableName> *table_names;
        Exp *whereExp;
    };
}
#endif //SELECT_STMT_H
