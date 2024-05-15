#include <iostream>
#include <ostream>

#include "sql/exp.h"
#include "execution/executor.h"
//
// Created by @ruitianzhong on 5/15/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//
namespace xDB {
    static bool checkIsSameType(Exp *exp1, Exp *exp2) {
        return exp1->getValue().getType() == exp2->getValue().getType();
    }

    bool ExpProcessor::process(BinaryExp *binary_exp) {
        if (!checkIsSameType(binary_exp->getLeft(), binary_exp->getRight())) {
            return false;
        }
        switch (binary_exp->binaryType()) {
            case BinaryAdd:
                break;
            case BinaryMul:
                break;
            case BinaryDiv:
                break;
            case BinarySub:
                break;
            case BinaryEquals:
                break;
            case BinaryXOR:
                break;
            case BinaryGreat:
                break;
            case BinaryLess:
                break;
            case BinaryGreatEQ:
                break;
            case BinaryOR:
                break;
            case BinaryModulo:
                break;
            case BinaryNotEquals:
                break;
            case BinaryAND:
                break;
            case BinaryLessEQ:
                break;
            default:
                std::cout << "Unknown binary exp" << std::endl;
                return false;
        }
        return false;
    }
}
