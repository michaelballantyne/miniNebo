#include "Core.h"

// Write field

std::ostream & operator<<(std::ostream & os, Field & field) {
    return core_write_field(os, field);
}

// Wrap function

const ConstExpr wrap(double valueArg) {
    return ConstExpr(valueArg);
}

const ConstExpr wrap(int valueArg) {
    return ConstExpr(valueArg);
}

const FieldExpr<Field> wrap(const Field & arg) {
    return FieldExpr<Field>(arg);
}

template<typename Expr>
const Expr & wrap(const Expr & arg) {
    return arg;
}

// Wrap return metafunction

template<typename Expr>
struct WrapReturn {
    Expr typedef result;
};

template<>
struct WrapReturn<double> {
    ConstExpr typedef result;
};

template<>
struct WrapReturn<Field> {
    FieldExpr<Field> typedef result;
};

template<>
struct WrapReturn<int> {
    ConstExpr typedef result;
};

// Nebo assignment

template<typename Expr>
void operator<<=(Field & field, const Expr & rhs) {
    core_assign(field, wrap(rhs));
}

// Interface functions

template<typename Arg1, typename Arg2>
BinExpr<SumOp, typename WrapReturn<Arg1>::result, typename WrapReturn<Arg2>::result> operator+(const Arg1 & arg1, const Arg2 & arg2) {
    return BinExpr<SumOp, typename WrapReturn<Arg1>::result, typename WrapReturn<Arg2>::result>(wrap(arg1), wrap(arg2));
}

// Interface functions

template<typename Arg1, typename Arg2>
BinExpr<MultOp, typename WrapReturn<Arg1>::result, typename WrapReturn<Arg2>::result> operator*(const Arg1 & arg1, const Arg2 & arg2) {
    return BinExpr<MultOp, typename WrapReturn<Arg1>::result, typename WrapReturn<Arg2>::result>(wrap(arg1), wrap(arg2));
}
