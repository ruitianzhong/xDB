//
// Created by rtzhong on 5/10/24.
//

#ifndef EXP_H
#define EXP_H

namespace xDB {
    class Exp {
    };

    class Operand : public Exp {
    };

    class LogicExpr : public Exp {
    };

    class BetweenExpr : public Exp {
    };

    class CompExpr : public Operand {
    };

    class BinaryExp {
    };

    class UnaryExp {
    };

    class ScalarExp {
    };

    enum ScalarType {
        ScalarLiteral,
        ScalarName,
    };
}
#endif //EXP_H
