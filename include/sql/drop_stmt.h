//
// Created by rtzhong on 5/10/24.
//

#ifndef DROP_STMT_H
#define DROP_STMT_H
#include <string>

#include "stmt.h"
#include "table.h"

namespace xDB {
    enum DropType {
        DropTable,
        DropDatabase,
    };

    class DropStmt : public SQLStmt {
    public:
        explicit DropStmt(DropType type, TableName name_);

        TableName name{};
        DropType type;

    private:
    };
}

#endif //DROP_STMT_H
