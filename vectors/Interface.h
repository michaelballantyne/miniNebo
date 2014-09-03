#include "Core.h"

// Interface functions

BinExpr<SumOp,
        VectorExpr,
        VectorExpr>
operator+(const std::vector<double> & vector1, const std::vector<double> & vector2) {
    BinExpr<SumOp,
            VectorExpr,
            VectorExpr> typedef ReturnType;

    return ReturnType(vector1, vector2);
}

template<typename SubExpr>
BinExpr<SumOp,
        SubExpr,
        VectorExpr>
operator+(const SubExpr & subExpr, const std::vector<double> & vector) {
    BinExpr<SumOp,
            SubExpr,
            VectorExpr> typedef ReturnType;

    return ReturnType(subExpr, vector);
}
