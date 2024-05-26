#include "execution/executor.h"
//
// Created by @ruitianzhong on 5/26/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
namespace xDB {
    bool ExpEvaluator::process(BetweenExpr *between_expr) {
        assert(between_expr!=nullptr);
        const Value v1 = between_expr->getExp1()->getValue(), v2 = between_expr->getExp2()->getValue(), v3 =
                between_expr->getExp3()->getValue();
        if (v1.getType() == ScalarInteger && v2.getType() == ScalarInteger && v3.getType() == ScalarInteger) {
            int result;
            if (v1.getInteger() >= v2.getInteger() && v1.getInteger() <= v3.getInteger()) {
                result = 1;
            } else {
                result = 0;
            }
            between_expr->setValue(Value(result));
            return true;
        }
        if (v1.getType() == ScalarChar && v2.getType() == ScalarChar && v3.getType() == ScalarChar) {
            const std::string s1 = v1.getChar(), s2 = v2.getChar(), s3 = v3.getChar();
            int result;
            if (s1.compare(s2) >= 0 && s1.compare(s3) <= 0) {
                result = 1;
            } else {
                result = 0;
            }
            between_expr->setValue(Value(result));
            return true;
        }
        return false;
    }
}
