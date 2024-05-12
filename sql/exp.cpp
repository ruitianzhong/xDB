//
// Created by @ruitianzhong on 5/12/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
#include "sql/exp.h"

namespace xDB {
    Exp::~Exp() = default;

    Exp::Exp(ExpType type_): expType(type_) {
    }

    BinaryExpType BinaryExp::binaryType() const {
        return type;
    }

    BinaryExp::BinaryExp(BinaryExpType type_, Exp *left_, Exp *right_) : Exp(ExpBinary), type(type_), left(left_),
                                                                         right(right_) {
    }

    UnaryExpType UnaryExp::unaryType() const {
        return type;
    }

    UnaryExp::UnaryExp(UnaryExpType type_, Exp *exp_): Exp(ExpUnary), exp(exp_), type(type_) {
    }

    BetweenExpr::BetweenExpr(Exp *exp1_, Exp *exp2_, Exp *exp3_): Exp(ExpBetween), exp1(exp1_), exp2(exp2_),
                                                                  exp3(exp3_) {
    }

    ScalarExp::ScalarExp(): Exp(ExpScalar), type(ScalarNULL), column_name(nullptr), str(nullptr), integer(0), d(0) {
    }

    ScalarExp::ScalarExp(char *s): ScalarExp() {
        this->str = s;
    }

    ScalarExp::ScalarExp(double d_) : ScalarExp() {
        this->d = d_;
    }

    ScalarExp::ScalarExp(int i_): ScalarExp() {
        integer = i_;
    }

    ScalarType ScalarExp::scalarType() const {
        return type;
    }

    ScalarExp::ScalarExp(ColumnName *column_name_) : ScalarExp() {
        column_name = column_name_;
    }
}
