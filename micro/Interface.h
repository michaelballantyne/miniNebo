#include "Core.h"

// Interface functions

// Sum

BinExpr<SumOp,
        FieldExpr,
        FieldExpr>
operator+(const Field & field1, const Field & field2) {
    BinExpr<SumOp,
            FieldExpr,
            FieldExpr> typedef ReturnType;

    return ReturnType(field1, field2);
}

template<typename SubExpr1, typename SubExpr2>
BinExpr<SumOp,
        SubExpr1,
        SubExpr2>
operator+(const SubExpr1 & subExpr1, const SubExpr2 & subExpr2) {
    BinExpr<SumOp,
            SubExpr1,
            SubExpr2> typedef ReturnType;

    return ReturnType(subExpr1, subExpr2);
}

template<typename SubExpr>
BinExpr<SumOp,
        SubExpr,
        FieldExpr>
operator+(const SubExpr & subExpr, const Field & field) {
    BinExpr<SumOp,
            SubExpr,
            FieldExpr> typedef ReturnType;

    return ReturnType(subExpr, field);
}

template<typename SubExpr>
BinExpr<SumOp,
        FieldExpr,
        SubExpr>
operator+(const Field & field, const SubExpr & subExpr) {
    BinExpr<SumOp,
            FieldExpr,
            SubExpr> typedef ReturnType;

    return ReturnType(field, subExpr);
}

// Multiplication

BinExpr<MultOp,
        FieldExpr,
        FieldExpr>
operator*(const Field & field1, const Field & field2) {
    BinExpr<MultOp,
            FieldExpr,
            FieldExpr> typedef ReturnType;

    return ReturnType(field1, field2);
}

template<typename SubExpr1, typename SubExpr2>
BinExpr<MultOp,
        SubExpr1,
        SubExpr2>
operator*(const SubExpr1 & subExpr1, const SubExpr2 & subExpr2) {
    BinExpr<MultOp,
            SubExpr1,
            SubExpr2> typedef ReturnType;

    return ReturnType(subExpr1, subExpr2);
}

template<typename SubExpr>
BinExpr<MultOp,
        SubExpr,
        FieldExpr>
operator*(const SubExpr & subExpr, const Field & field) {
    BinExpr<MultOp,
            SubExpr,
            FieldExpr> typedef ReturnType;

    return ReturnType(subExpr, field);
}

template<typename SubExpr>
BinExpr<MultOp,
        FieldExpr,
        SubExpr>
operator*(const Field & field, const SubExpr & subExpr) {
    BinExpr<MultOp,
            FieldExpr,
            SubExpr> typedef ReturnType;

    return ReturnType(field, subExpr);
}
