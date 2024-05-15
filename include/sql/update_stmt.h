//
// Created by rtzhong on 5/10/24.
//

#ifndef UPDATE_STMT_H
#define UPDATE_STMT_H
#include "exp.h"
#include "stmt.h"
#include "table.h"
#include <vector>

namespace xDB {
    class UpdateAssign {
    public:
        UpdateAssign(ColumnName *column_name_, Exp *right_);

        ColumnName *column_name;

        Exp *right;
    };

    class UpdateStmt : public SQLStmt {
    public:
        UpdateStmt(TableName table_name_, std::vector<UpdateAssign *> *list_, Exp *whereExp_);

        ~UpdateStmt() override;

        TableName table_name;
        std::vector<UpdateAssign *> *update_assigns;
        Exp *whereExp;
    };
}
#endif //UPDATE_STMT_H
