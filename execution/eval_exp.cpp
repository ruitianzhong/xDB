//
// Created by @ruitianzhong on 5/16/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//


#include <utility>

#include "execution/executor.h"

namespace xDB {
    bool Executor::evaluateBetween(BetweenExpr *between_expr, ExpProcessor *processor) {
        assert(between_expr!=nullptr);
        if (!evaluateExp(between_expr->getExp1(), processor) || !evaluateExp(between_expr->getExp2(), processor) || !
            evaluateExp(
                between_expr->getExp3(), processor)) {
            return false;
        }
        return between_expr->visit(processor);
    }

    bool Executor::evaluateBinary(BinaryExp *exp, ExpProcessor *processor) {
        // NOLINT(*-no-recursion)
        assert(exp!=nullptr);
        if (!evaluateExp(exp->getLeft(), processor) || !evaluateExp(exp->getRight(), processor)) {
            return false;
        }
        return exp->visit(processor);
    }

    bool Executor::evaluateScalar(ScalarExp *exp, ExpProcessor *processor) {
        assert(exp!=nullptr);
        return exp->visit(std::move(processor));
    }

    bool Executor::evaluateUnary(UnaryExp *exp, ExpProcessor *processor) {
        // NOLINT(*-no-recursion)
        if (!evaluateExp(exp->getExp(), processor)) {
            return false;
        }
        return exp->visit(processor);
    }


    bool Executor::evaluateExp(Exp *exp, ExpProcessor *processor) {
        assert(exp!=nullptr);

        switch (exp->getExpType()) {
            case ExpBetween:
                return evaluateBetween(dynamic_cast<BetweenExpr *>(exp), processor);
            case ExpBinary:
                return evaluateBinary(dynamic_cast<BinaryExp *>(exp), processor);
            case ExpScalar:
                return evaluateScalar(dynamic_cast<ScalarExp *>(exp), processor);
            case ExpUnary:
                return evaluateUnary(dynamic_cast<UnaryExp *>(exp), processor);
            default:
                std::cout << "Something wrong here" << std::endl;
                return false;
        }
    }
}
