#include "execution/executor.h"
//
// Created by @ruitianzhong on 5/16/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
namespace xDB {
    bool ExpChecker::process(ScalarExp *scalar_exp) {
        if (scalar_exp->getType() != ScalarName) {
            return true;
        }
        const ColumnName *column_name = scalar_exp->getColumnName();

        assert(column_name!=nullptr);
        // check column name
        const std::string cn = column_name->column_name;

        auto m = context_.fullname2index();
        if (column_name->name == nullptr) {
            bool found = false;
            ColumnFullName full_name;
            for (auto &i: m) {
                if (i.first.column_name == cn) {
                    if (found) {
                        std::cout << "ambiguous column name " << cn << std::endl;
                        return false;
                    }
                    full_name = i.first;
                    found = true;
                }
            }
            if (!found) {
                std::cout << "Can not infer column name " << cn << std::endl;
                return false;
            }
            scalar_exp->setFullColumnName(full_name);
            return true;
        }
        if (column_name->name != nullptr) {
            bool found = false;
            ColumnFullName full_name;

            for (auto &i: m) {
                if (i.first.column_name == column_name->column_name && i.first.table_name == column_name->name) {
                    if (found) {
                        std::cout << "Ambiguous column " << column_name->name << "." << column_name->column_name <<
                                std::endl;
                        return false;
                    }
                    found = true;
                    full_name = i.first;
                }
            }

            if (!found) {
                std::cout << "Unknown column " << column_name->column_name << std::endl;
                return false;
            }
            scalar_exp->setFullColumnName(full_name);
            return true;
        }
        if (column_name->schema != nullptr) {
            const auto fullname = ColumnFullName(column_name->schema, column_name->name, column_name->column_name);

            if (m.find(fullname) == m.end()) {
                std::cout << fullname.toString() << " does not exist" << std::endl;
                return false;
            }
            scalar_exp->setFullColumnName(fullname);
            return true;
        }
        std::cout << __FUNCTION__ << " unexpected error " << std::endl;

        return false;
    }
}
