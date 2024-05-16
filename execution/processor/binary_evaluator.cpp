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


    static bool processNumeric(BinaryExp *exp, const Value &left, const Value &right) {
        if (left.getType() != ScalarInteger || right.getType() != ScalarInteger) {
            std::cout << "Warning : Not a number" << std::endl;
            exp->setValue(Value::MakeNullValue());
            return true;
        }
        const int lv = left.getInteger(), rv = right.getInteger();
        int result;
        switch (exp->binaryType()) {
            case BinaryAdd:
                result = lv + rv;
                break;
            case BinaryMul:
                result = lv * rv;
                break;
            case BinaryDiv:
                if (rv == 0) {
                    std::cout << "Divide by zero" << std::endl;
                    return false;
                }
                result = lv / rv;
                break;

            case BinarySub:
                result = lv - rv;
                break;
            case BinaryXOR:
                result = lv ^ rv;
                break;
            case BinaryOR:
                result = lv | rv;
                break;
            case BinaryModulo:
                if (rv == 0) {
                    std::cout << "Divide by zero error" << std::endl;
                    return false;
                }
                result = lv % rv;
                break;
            default:
                assert(false);
        }
        const Value v(result);
        exp->setValue(v);
        return true;
    }

    static int transfer(bool ok) {
        if (ok) { return 1; }
        return 0;
    }

    static bool processIntegerRelExp(BinaryExp *binary_exp, const Value &left, const Value &right) {
        const int lv = left.getInteger(), rv = right.getInteger();
        int result;
        switch (binary_exp->binaryType()) {
            case BinaryEquals:
                result = transfer(lv == rv);
                break;
            case BinaryGreat:
                result = transfer(lv > rv);
                break;
            case BinaryLess:
                result = transfer(lv < rv);
                break;
            case BinaryGreatEQ:
                result = transfer(lv >= rv);
                break;
            case BinaryNotEquals:
                result = transfer(lv != rv);
                break;
            case BinaryAND:
                result = transfer(lv != 0 && rv != 0);
                break;
            case BinaryLessEQ:
                result = transfer(lv <= rv);
                break;
            case BinaryOR:
                result = transfer(lv != 0 || rv != 0);
                break;
            default:
                assert(false);
        }

        binary_exp->setValue(Value(result));
        return true;
    }

    static bool processCharRelExp(BinaryExp *binary_exp, const Value &left, const Value &right) {
        const std::string lv = left.getChar(), rv = right.getChar();
        int result;
        switch (binary_exp->binaryType()) {
            case BinaryEquals:
                result = transfer(lv == rv);
                break;
            case BinaryGreat:
                result = transfer(lv > rv);
                break;
            case BinaryLess:
                result = transfer(lv < rv);
                break;
            case BinaryGreatEQ:
                result = transfer(lv >= rv);
                break;
            case BinaryNotEquals:
                result = transfer(lv != rv);
                break;
            case BinaryAND:
                std::cout << "Op `AND` is not supported on CHAR" << std::endl;
                return false;
            case BinaryLessEQ:
                result = transfer(lv <= rv);
                break;
            case BinaryOR:
                std::cout << "Op `OR` is not supported on CHAR" << std::endl;
                return false;
            default:
                assert(false);
        }

        binary_exp->setValue(Value(result));
        return true;
    }

    bool ExpEvaluator::process(BinaryExp *binary_exp) {
        if (!checkIsSameType(binary_exp->getLeft(), binary_exp->getRight())) {
            return false;
        }

        Value left = binary_exp->getLeft()->getValue(), right = binary_exp->getRight()->getValue();
        switch (binary_exp->binaryType()) {
            case BinaryAdd:
            case BinaryMul:
            case BinaryDiv:
            case BinarySub:
            case BinaryXOR:
            case BinaryModulo:
                return processNumeric(binary_exp, left, right);
            case BinaryEquals:
            case BinaryGreat:
            case BinaryLess:
            case BinaryGreatEQ:
            case BinaryNotEquals:
            case BinaryAND:
            case BinaryLessEQ:
            case BinaryOR:
                if (left.getType() == ScalarInteger && right.getType() == ScalarInteger) {
                    return processIntegerRelExp(binary_exp, left, right);
                }
                if (left.getType() == ScalarChar && right.getType() == ScalarChar) {
                    return processCharRelExp(binary_exp, left, right);
                }
                binary_exp->setValue(Value::MakeNullValue());
                return true;

            // Note that NULL seems to be incomparable in MySQL.
            default:
                std::cout << "Unknown binary exp" << std::endl;
                return false;
        }
    }
}
