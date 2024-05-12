//
// Created by rtzhong on 5/10/24.
//

#ifndef DELETE_STMT_H
#define DELETE_STMT_H
#include "exp.h"
#include "stmt.h"

namespace xDB {
    class DeleteStmt final : public SQLStmt {
    public:
        DeleteStmt(TableName table_name_, Exp *whereExp_);

        ~DeleteStmt() override;

    private:
        TableName table_name;
        Exp *whereExp;
    };
}
#endif //DELETE_STMT_H
