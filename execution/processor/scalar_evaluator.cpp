#include <iostream>
#include <ostream>

#include "sql/exp.h"
#include "execution/executor.h"
//
// Created by @ruitianzhong on 5/15/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
namespace xDB {
    bool ExpEvaluator::process(ScalarExp *scalar_exp) {
        assert(scalar_exp!=nullptr);
        switch (scalar_exp->scalarType()) {
            case ScalarChar:
                scalar_exp->setValue(Value(scalar_exp->getStr()));
                return true;
            case ScalarInteger:
                scalar_exp->setValue(Value(scalar_exp->getInteger()));
                return true;
            case ScalarName:
                return processScalarName(scalar_exp);
            case ScalarNULL:
                scalar_exp->setValue(Value::MakeNullValue());
                return true;
            default:
                std::cout << "Unsupported scalor" << std::endl;
                return false;
        }
    }

    bool ExpEvaluator::processScalarName(ScalarExp *scalar_exp) {
        auto m = context_.fullname2index();
        if (m.find(scalar_exp->getColumnFullName()) == m.end()) {
            std::cout << "Unexpected error: " << std::endl;
            return false;
        }
        const int index = m[scalar_exp->getColumnFullName()];

        Column column = context_.column(index);
        switch (column.type()) {
            case Column::COLUMN_INT:
                scalar_exp->setValue(Value(column.integer_num()));
                return true;
            case Column::COLUMN_CHAR:
                scalar_exp->setValue(Value(column.str()));
                return true;
            case Column::COLUMN_NULL:
                scalar_exp->setValue(Value::MakeNullValue());
                return true;
            default:
                std::cout << "Unsupported scalar" << std::endl;
                return false;
        }
    }
}
