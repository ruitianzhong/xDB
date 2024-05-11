//
// Created by rtzhong on 5/10/24.
//

#ifndef STMT_H
#define STMT_H

namespace xDB {
    enum SQLStmtType {
        xSQL_SELECT,
        xSQL_INSERT,
        xSQL_UPDATE,
        xSQL_CREATE,
        xSQL_DELETE,
        xSQL_DROP,
        xSQL_USE,
        xSQL_SHOW,
    };

    class SQLStmt {
    public:
        explicit SQLStmt(SQLStmtType type);

        SQLStmtType type() const;

        virtual ~SQLStmt();

    private:
        SQLStmtType type_;
    };
}


#endif //STMT_H
