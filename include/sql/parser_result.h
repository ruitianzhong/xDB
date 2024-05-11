//
// Created by @ruitianzhong on 5/11/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//

#ifndef PARSER_RESULT_H
#define PARSER_RESULT_H
#include <vector>

#include "stmt.h"

namespace xDB {
    class ParserResult {
    public:
        ParserResult() = default;

        void addStatement(SQLStmt *stmt) {
            statements_.push_back(stmt);
        }

    private:
        std::vector<SQLStmt *> statements_;
    };
}


#endif //PARSER_RESULT_H
