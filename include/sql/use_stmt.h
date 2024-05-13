//
// Created by rtzhong on 5/10/24.
//

#ifndef USE_STMT_H
#define USE_STMT_H
#include "stmt.h"
#include "table.h"

namespace xDB {
    class UseStmt final : public SQLStmt {
    public:
        explicit UseStmt(TableName name_);

        ~UseStmt() override;

        TableName name{};
    };
}
#endif //USE_STMT_H
