#include "sql/insert_stmt.h"
#include <cstdlib>
//
// Created by @ruitianzhong on 5/11/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
namespace xDB {
    DataType Parameter::getType() const {
        return type;
    }

    Parameter::Parameter(char *str_): str(str_), type(FLOAT) {
        x = 0;
        d = 0;
    }

    Parameter::Parameter(const double d_) : d(d_), type(FLOAT) {
        str = nullptr;
        x = 0;
    }

    Parameter::Parameter(const int x_) : x(x_), type(INTEGER) {
        str = nullptr;
        d = 0;
    }

    char *Parameter::getChar() const {
        return str;
    }

    double Parameter::getDouble() const {
        return d;
    }

    int Parameter::getInt() const {
        return x;
    }

    Parameter::~Parameter() {
        if (type == CHAR) {
            free(str);
        }
    }

    InsertStmt::InsertStmt(TableName table_name, std::vector<ColumnName *> *optColumnName,
                           std::vector<Parameter *> *para): SQLStmt(xSQL_INSERT) {

    }

    InsertStmt::~InsertStmt() = default;
}