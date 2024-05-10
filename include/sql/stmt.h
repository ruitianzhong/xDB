//
// Created by rtzhong on 5/10/24.
//

#ifndef STMT_H
#define STMT_H

namespace xDB {
    class SQLStmt {
    };

    enum SQLStmtType {
        SQL_SELECT,
        SQL_INSERT,
        SQL_UPDATE,
        SQL_CREATE,
        SQL_DELETE,
        SQL_DROP,
        SQL_USE,
        SQL_SHOW,
    };
}


#endif //STMT_H
