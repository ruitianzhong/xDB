#include <iostream>

#include "sql/exp.h"
#include "execution/executor.h"
//
// Created by @ruitianzhong on 5/15/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
namespace xDB {
    bool ExpEvaluator::processNegate(UnaryExp *exp) {
        if (exp->getExp()->getValue().getType() != ScalarInteger) {
            std::cout << "incompatible type for negation " << std::endl;
            return false;
        }
        const int v = exp->getValue().getInteger();
        const Value value(-v);
        exp->setValue(value);
        return true;
    }

    bool ExpEvaluator::processNot(UnaryExp *exp) {
        if (exp->getExp()->getValue().getType() != ScalarInteger) {
            std::cout << "incompatible type for NOT" << std::endl;
            return false;
        }
        int integer = exp->getValue().getInteger();
        Value v;
        if (integer == 0) {
            v = Value(1);
        } else {
            v = Value(0);
        }
        exp->setValue(v);
        return true;
    }

    bool ExpEvaluator::processIsNull(UnaryExp *exp) {
        Value v;
        if (exp->getExp()->getValue().getType() != ScalarNULL) {
            v = Value(0);
        } else {
            v = Value(1);
        }
        exp->setValue(v);
        return true;
    }

    bool ExpEvaluator::processIsNotNull(UnaryExp *exp) {
        Value v;
        if (exp->getExp()->getValue().getType() != ScalarNULL) {
            v = Value(1);
        } else {
            v = Value(0);
        }
        exp->setValue(v);
        return true;
    }


    bool ExpEvaluator::process(UnaryExp *unary_exp) {
        switch (unary_exp->unaryType()) {
            case UnaryNegate:
                return processNegate(unary_exp);
            case UnaryIsNull:
                return processIsNull(unary_exp);
            case UnaryNot:
                return processNot(unary_exp);
            case UnaryIsNotNull:
                return processIsNotNull(unary_exp);
            default:
                std::cout << "Unknown UnaryType" << std::endl;
                return false;
        }
    }
}
