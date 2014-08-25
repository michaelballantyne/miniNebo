#include "Core.h"

// Printing fields to streams
template<typename Location>
std::ostream & operator<<(std::ostream & os, TypedField<Location> & field) {
    return core_write_field(os, field);
}

// Typed expression

template<typename Location, typename Expr>
struct TypedExpr {
    Location typedef location;
    Expr typedef expr_type;

    const Expr expr;

    TypedExpr(const Expr & exprArg) : expr(exprArg) {}
};

// Types

struct SingleValue;
struct SVol;

// JoinLocation Metafunction

template<typename Location1, typename Location2>
struct JoinLocation;

template<typename Location>
struct JoinLocation<Location, Location> {
    Location typedef result;
};

template<>
struct JoinLocation<SingleValue, SingleValue> {
    SingleValue typedef result;
};

template<typename OtherLocation>
struct JoinLocation<SingleValue, OtherLocation> {
    OtherLocation typedef result;
};

template<typename OtherLocation>
struct JoinLocation<OtherLocation, SingleValue> {
    OtherLocation typedef result;
};

// CheckEqual metafunction

template<typename Type1, typename Type2>
struct CheckEqual;

template<typename Type>
struct CheckEqual<Type, Type> {
    Type typedef result;
};

// ExtractLocation metafunction

template<typename Term>
struct ExtractLocation {
    typename Term::location typedef result;
};

template<>
struct ExtractLocation<double> {
    SingleValue typedef result;
};

template<>
struct ExtractLocation<int> {
    SingleValue typedef result;
};

// Wrap function

const ConstExpr wrap(double valueArg) {
    return ConstExpr(valueArg);
}

const ConstExpr wrap(int valueArg) {
    return ConstExpr(valueArg);
}

template<typename Location>
const FieldExpr<Initial, TypedField<Location> > wrap(const TypedField<Location> & arg) {
    return FieldExpr<Initial, TypedField<Location> >(arg);
}

template<typename Location, typename ExprType>
const ExprType & wrap(const TypedExpr<Location, ExprType> & arg) {
    return arg.expr;
}

// Wrap return metafunction

template<typename FieldType>
struct WrapReturn {
    FieldExpr<Initial, FieldType> typedef result;
};

template<typename Location, typename ExprType>
struct WrapReturn<TypedExpr<Location, ExprType> > {
    ExprType typedef result;
};

template<>
struct WrapReturn<double> {
    ConstExpr typedef result;
};

template<>
struct WrapReturn<int> {
    ConstExpr typedef result;
};

// Nebo assignment

template<typename FieldType, typename TypedExpr>
void operator<<=(FieldType & field, const TypedExpr & rhs) {
    typename CheckEqual<typename JoinLocation<typename FieldType::location,
                                              typename ExtractLocation<TypedExpr>::result>::result,
                        typename FieldType::location>::result typedef check;
    core_assign(field, wrap(rhs));
}

// Generic interface function

template<typename Op, typename Arg1, typename Arg2>
struct BinExprType {
    BinExpr<Initial, Op, typename WrapReturn<Arg1>::result, typename WrapReturn<Arg2>::result> typedef result;
};

template<typename Op, typename Arg1, typename Arg2>
struct BinExprReturn {
    TypedExpr<typename JoinLocation<typename ExtractLocation<Arg1>::result, typename ExtractLocation<Arg2>::result>::result,
                       typename BinExprType<Op, Arg1, Arg2>::result > typedef result;
};

template<typename Op, typename Arg1, typename Arg2>
typename BinExprReturn<Op, Arg1, Arg2>::result binExpr(const Arg1 & arg1, const Arg2 & arg2) {
    typename BinExprType<Op, Arg1, Arg2>::result typedef ExprType;
    typename BinExprReturn<Op, Arg1, Arg2>::result typedef ReturnType;

    return ReturnType(ExprType(wrap(arg1), wrap(arg2)));
}

// Specific interface functions

template<typename Arg1, typename Arg2>
typename BinExprReturn<SumOp, Arg1, Arg2>::result operator+(const Arg1 & arg1, const Arg2 & arg2) {
    return binExpr<SumOp>(arg1, arg2);
}

template<typename Arg1, typename Arg2>
typename BinExprReturn<MultOp, Arg1, Arg2>::result operator*(const Arg1 & arg1, const Arg2 & arg2) {
    return binExpr<MultOp>(arg1, arg2);
}
