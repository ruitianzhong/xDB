//
// Created by @ruitianzhong on 5/16/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//


#include <utility>

#include "execution/executor.h"

namespace xDB {
    bool Executor::visitBetween(BetweenExpr *between_expr, AbstractExpProcessor *processor)const {// NOLINT(*-no-recursion)
        assert(between_expr!=nullptr);
        if (!visitExp(between_expr->getExp1(), processor) || !visitExp(between_expr->getExp2(), processor) || !
            visitExp(
                between_expr->getExp3(), processor)) {
            return false;
        }
        return between_expr->visit(processor);
    }

    bool Executor::visitBinary(BinaryExp *exp, AbstractExpProcessor *processor)const {// NOLINT(*-no-recursion)
        assert(exp!=nullptr);
        if (!visitExp(exp->getLeft(), processor) || !visitExp(exp->getRight(), processor)) {
            return false;
        }
        return exp->visit(processor);
    }

    bool Executor::visitScalar(ScalarExp *exp, AbstractExpProcessor *processor) const{
        assert(exp!=nullptr);
        return exp->visit(processor);
    }

    bool Executor::visitUnary(UnaryExp *exp, AbstractExpProcessor *processor)const { // NOLINT(*-no-recursion)
        // NOLINT(*-no-recursion)
        if (!visitExp(exp->getExp(), processor)) {
            return false;
        }
        return exp->visit(processor);
    }


    bool Executor::visitExp(Exp *exp, AbstractExpProcessor *processor) const { // NOLINT(*-no-recursion)
        assert(exp!=nullptr);

        switch (exp->getExpType()) {
            case ExpBetween:
                return visitBetween(dynamic_cast<BetweenExpr *>(exp), processor);
            case ExpBinary:
                return visitBinary(dynamic_cast<BinaryExp *>(exp), processor);
            case ExpScalar:
                return visitScalar(dynamic_cast<ScalarExp *>(exp), processor);
            case ExpUnary:
                return visitUnary(dynamic_cast<UnaryExp *>(exp), processor);
            default:
                std::cout << "Something wrong here" << std::endl;
                return false;
        }
    }
}
