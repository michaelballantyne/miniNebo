#include<cfloat>
#include<iostream>
#include<sstream>
#include<stdexcept>

#include "Field.h"

// Field expressions

template<typename FieldType>
class FieldExpr {
    private:
        const FieldType & field;

    public:
        FieldExpr(const FieldType & fieldArg) : field(fieldArg) {}

        double eval(int x, int y, int z) const {
            return field.get_data()[z * field.dim.x * field.dim.y + y * field.dim.x + x];
        }
};

// Constant expressions

struct ConstExpr {
    const double value;

    ConstExpr(const double valueArg) : value(valueArg) {}

    double eval(int x, int y, int z) const {
        return value;
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
void core_assign(FieldType & field, const Expr & expr) {
    double* data = field.get_data();

    for (int z = 0; z < field.dim.z; z++) {
        for (int y = 0; y < field.dim.y; y++) {
            for (int x = 0; x < field.dim.x; x++) {
                data[z * field.dim.y * field.dim.x + y * field.dim.x + x] = expr.eval(x, y, z);
            }
        }
    }
}


#pragma optimization_level 0
template<typename FieldType>
std::ostream & core_write_field(std::ostream & os, const FieldType & field)
{
    FieldExpr<FieldType> expr = FieldExpr<FieldType>(field);
    for (int z = 0; z < field.dim.z; z++) {
        os << "z = " << z << std::endl;
        for (int y = 0; y < field.dim.y; y++) {
            for (int x = 0; x < field.dim.x; x++) {
                os << expr.eval(x, y, z) << " ";
            }
            os << std::endl;
        }
        os << std::endl;
    }

    return os;
}

