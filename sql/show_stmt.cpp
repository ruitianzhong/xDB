//
// Created by @ruitianzhong on 5/11/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//

#include "sql/show_stmt.h"

namespace xDB {
    ShowStmt::ShowStmt(const ShowType show_type_): SQLStmt(xSQL_SHOW), show_type(show_type_) {
    }

    ShowType ShowStmt::getShowType() const {
        return show_type;
    }

    ShowStmt::~ShowStmt() = default;
}
