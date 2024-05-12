//
// Created by rtzhong on 5/10/24.
//

#ifndef EXP_H
#define EXP_H
#include "table.h"

namespace xDB {
    enum UnaryExpType {
        UnaryNegate,
        UnaryIsNull,
        UnaryIsNotNull,
        UnaryNot,
    };

    enum BinaryExpType {
        BinaryAdd,
        BinaryMul,
        BinaryDiv,
        BinarySub,
        BinaryModulo,
        BinaryXOR,
        BinaryEquals,
        BinaryNotEquals,
        BinaryGreatEQ,
        BinaryLessEQ,
        BinaryGreat,
        BinaryLess,
        BinaryAND,
        BinaryOR,
    };


    enum ExpType {
        ExpBinary,
        ExpUnary,
        ExpBetween,
        ExpScalar,
    };

    class Exp {
    public:
        virtual ~Exp();

    protected:
        explicit Exp(ExpType type_);

    private:
        ExpType expType;
    };


    class BetweenExpr : public Exp {
    public:
        BetweenExpr(Exp *exp1_, Exp *exp2_, Exp *exp3_);

    private:
        Exp *exp1, *exp2, *exp3;
    };


    class BinaryExp : public Exp {
    public:
        BinaryExp(BinaryExpType type_, Exp *left_, Exp *right_);

        BinaryExpType binaryType() const;

    private:
        BinaryExpType type;
        Exp *left, *right;
    };

    class UnaryExp : public Exp {
    public:
        UnaryExp(UnaryExpType type_, Exp *exp_);

        UnaryExpType unaryType() const;

    private:
        Exp *exp;
        UnaryExpType type;
    };

    enum ScalarType {
        ScalarChar,
        ScalarName,
        ScalarFloat,
        ScalarNULL,
    };

    class ScalarExp : public Exp {
    public:
        explicit ScalarExp(ColumnName *column_name_);

        explicit ScalarExp(char *);

        explicit ScalarExp(int);

        explicit ScalarExp();

        explicit ScalarExp(double);

        ScalarType scalarType() const;

    private:
        ScalarType type;
        ColumnName *column_name;
        char *str;
        int integer;
        double d;
    };
}
#endif //EXP_H
