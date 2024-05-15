//
// Created by @ruitianzhong on 5/12/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
#include "sql/exp.h"

#include <utility>

#include "execution/executor.h"

namespace xDB {
    Exp::~Exp() = default;

    Exp::Exp(ExpType type_): expType(type_) {
    }

    void Exp::setValue(Value v) {
        this->value_ = std::move(v);
    }

    Value Exp::getValue() {
        return value_;
    }


    BinaryExpType BinaryExp::binaryType() const {
        return type;
    }

    BinaryExp::BinaryExp(BinaryExpType type_, Exp *left_, Exp *right_) : Exp(ExpBinary), type(type_), left(left_),
                                                                         right(right_) {
    }

    void BinaryExp::visit(ExpProcessor processor) {
        processor.process(this);
    }


    UnaryExpType UnaryExp::unaryType() const {
        return type;
    }

    void UnaryExp::visit(ExpProcessor processor) {
        processor.process(this);
    }


    UnaryExp::UnaryExp(UnaryExpType type_, Exp *exp_): Exp(ExpUnary), exp(exp_), type(type_) {
    }

    BetweenExpr::BetweenExpr(Exp *exp1_, Exp *exp2_, Exp *exp3_): Exp(ExpBetween), exp1(exp1_), exp2(exp2_),
                                                                  exp3(exp3_) {
    }

    void BetweenExpr::visit(ExpProcessor processor) {
        processor.process(this);
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

    void ScalarExp::visit(ExpProcessor processor) {
        processor.process(this);
    }

    Value::Value() : type_(ScalarInvalid), integer_num(0) {
    }

    Value Value::MakeNullValue() {
        Value v;
        v.type_ = ScalarNULL;
        return v;
    }

    Value::Value(const int x) : type_(ScalarInteger), integer_num(x) {
    }

    Value::Value(std::string s): type_(ScalarChar), s_(std::move(s)), integer_num(0) {
    }

    std::string Value::getChar() const {
        return s_;
    }

    int Value::getInteger() const {
        return integer_num;
    }

    ScalarType Value::getType() const {
        return type_;
    }
}
