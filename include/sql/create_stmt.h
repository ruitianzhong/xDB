//
// Created by rtzhong on 5/10/24.
//

#ifndef CREATE_STMT_H
#define CREATE_STMT_H
#include "stmt.h"

namespace xDB {
    enum CreateStmtType {
        DATABASE,
        TABLE
    };

    class CreateStmt : SQLStmt {
    };
}
#endif //CREATE_STMT_H
