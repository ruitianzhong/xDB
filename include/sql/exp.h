//
// Created by rtzhong on 5/10/24.
//

#ifndef EXP_H
#define EXP_H
#include <string>
#include "table.h"

namespace xDB {
    // forward declaration
    class ExpProcessor;

    enum ScalarType {
        ScalarChar,
        ScalarName,
        ScalarFloat,
        ScalarNULL,
        ScalarInteger,
        ScalarInvalid,
    };

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

    class Value {
    public:
        Value(); // Invalid

        explicit Value(int x); // integer
        explicit Value(std::string s);

        static Value MakeNullValue();

        [[nodiscard]] ScalarType getType() const;

        [[nodiscard]] std::string getChar() const;

        [[nodiscard]] int getInteger() const;

    private:
        ScalarType type_;
        std::string s_; // full column name
        int integer_num;
    };

    class Exp {
    public:
        virtual ~Exp();

        virtual void visit(ExpProcessor processor) =0;

        void setValue(Value v);

        [[nodiscard]] Value getValue();

    protected:
        explicit Exp(ExpType type_);

        Value value_;

    private:
        ExpType expType;
    };


    class BetweenExpr : public Exp {
    public:
        BetweenExpr(Exp *exp1_, Exp *exp2_, Exp *exp3_);

        void visit(ExpProcessor processor) override;

        [[nodiscard]] Exp *getExp1() const { return exp1; }
        [[nodiscard]] Exp *getExp2() const { return exp2; }
        [[nodiscard]] Exp *getExp3() const { return exp3; }

    private:
        Exp *exp1, *exp2, *exp3;
    };


    class BinaryExp : public Exp {
    public:
        BinaryExp(BinaryExpType type_, Exp *left_, Exp *right_);

        [[nodiscard]] BinaryExpType binaryType() const;

        void visit(ExpProcessor processor) override;

        [[nodiscard]] Exp *getLeft() const { return left; }
        [[nodiscard]] Exp *getRight() const { return right; }

    private:
        BinaryExpType type;
        Exp *left, *right;
    };

    class UnaryExp : public Exp {
    public:
        UnaryExp(UnaryExpType type_, Exp *exp_);

        [[nodiscard]] UnaryExpType unaryType() const;

        void visit(ExpProcessor processor) override;

    private:
        Exp *exp;
        UnaryExpType type;
    };


    class ScalarExp : public Exp {
    public:
        explicit ScalarExp(ColumnName *column_name_);

        explicit ScalarExp(char *);

        explicit ScalarExp(int);

        explicit ScalarExp();

        explicit ScalarExp(double);

        [[nodiscard]] ScalarType scalarType() const;

        void visit(ExpProcessor processor) override;

    private:
        ScalarType type;
        ColumnName *column_name;
        char *str;
        int integer;
        double d;
    };

    class ExecutionContext {
    };
}
#endif //EXP_H
