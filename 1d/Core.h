#include<cfloat>
#include<iostream>
#include<sstream>
#include<stdexcept>

#include "Field.h"

// Field expressions

class FieldExpr {
    private:
        const Field & field;

    public:
        FieldExpr(const Field & fieldArg) : field(fieldArg) {}

        int eval(int i) const {
            return field(i);
        }
};

// Binary expressions

template<typename Functor, typename SubExpr1, typename SubExpr2>
class BinExpr {
    private:
        const SubExpr1 & subExpr1;
        const SubExpr2 & subExpr2;
        Functor functor;

    public:
        BinExpr(const SubExpr1 & subExpr1Arg, const SubExpr2 & subExpr2Arg)
            : subExpr1(subExpr1Arg), subExpr2(subExpr2Arg) {}

        int eval(int i) const {
            return functor(subExpr1.eval(i), subExpr2.eval(i));
        }
};

// Operators

struct SumOp {
    int operator() (int lhs, int rhs) const {
        return lhs + rhs;
    }
};

struct MultOp {
    int operator() (int lhs, int rhs) const {
        return lhs * rhs;
    }
};


// Assignment

template<typename FieldType, typename Expr>
void operator<<=(FieldType & field, const Expr & expr) {
    for (int i = 0; i <  field.size; i++) {
        field(i) = expr.eval(i);
    }
}
