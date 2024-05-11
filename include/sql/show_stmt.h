//
// Created by rtzhong on 5/10/24.
//

#ifndef SHOW_STMT_H
#define SHOW_STMT_H
#include "stmt.h"

namespace xDB {
    enum ShowType {
        ShowDatabases,
        ShowTables,
    };

    class ShowStmt final : public SQLStmt {
    public:
        explicit ShowStmt(ShowType show_type_);

        ~ShowStmt() override;

    private:
        ShowType show_type;
    };
}
#endif //SHOW_STMT_H
