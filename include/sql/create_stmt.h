//
// Created by rtzhong on 5/10/24.
//

#ifndef CREATE_STMT_H
#define CREATE_STMT_H
#include <vector>

#include "stmt.h"
#include "table.h"

namespace xDB {
    enum CreateStmtType {
        CreateDatabase,
        CreateTable
    };

    class CreateStmt final : public SQLStmt {
    public:
        ~CreateStmt() override;

        explicit CreateStmt(TableName name_);

        CreateStmt(TableName name_, std::vector<DataDefinition *> *list_);

        TableName name;
        CreateStmtType createType;
        std::vector<DataDefinition *> *list;
    };
}
#endif //CREATE_STMT_H
