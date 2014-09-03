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

        double eval(int x, int y, int z) const {
            return field(x, y, z);
        }
};

// Binary expressions

template<typename Functor, typename SubExpr1, typename SubExpr2>
class BinExpr {
    private:
        const SubExpr1 subExpr1;
        const SubExpr2 subExpr2;
        Functor functor;

    public:
        BinExpr(const SubExpr1 & subExpr1Arg, const SubExpr2 & subExpr2Arg)
            : subExpr1(subExpr1Arg), subExpr2(subExpr2Arg) {}

        double eval(int x, int y, int z) const {
            return functor(subExpr1.eval(x, y, z), subExpr2.eval(x, y, z));
        }
};

// Operators

struct SumOp {
    double operator() (double lhs, double rhs) const {
        return lhs + rhs;
    }
};

struct MultOp {
    double operator() (double lhs, double rhs) const {
        return lhs * rhs;
    }
};


// Assignment

template<typename FieldType, typename Expr>
void operator<<=(FieldType & field, const Expr & expr) {
    for (int z = 0; z <  field.dim.z; z++) {
        for (int y = 0; y < field.dim.y; y++) {
            for (int x = 0; x < field.dim.x; x++) {
                field(x, y, z) = expr.eval(x, y, z);
            }
        }
    }
}

// Printing Fields to std::cout

std::ostream & operator<<(std::ostream & os, Field & field)
{
    for (int z = 0; z < field.dim.z; z++) {
        os << "z = " << z << std::endl;
        for (int y = 0; y < field.dim.y; y++) {
            for (int x = 0; x < field.dim.x; x++) {
                os << field(x, y, z) << " ";
            }
            os << std::endl;
        }
        os << std::endl;
    }

    return os;
}
