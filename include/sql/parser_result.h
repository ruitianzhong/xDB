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

        void addStatement(SQLStmt *stmt);

    private:
        std::vector<SQLStmt *> statements;
    };
}


#endif //PARSER_RESULT_H
