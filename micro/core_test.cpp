#include "Core.h"

#include<iostream>

int main(int argc, const char *argv[])
{
    Field a(Dimensions(8, 8, 1));
    Field b(Dimensions(8, 8, 1));
    Field c(Dimensions(8, 8, 1));
    Field d(Dimensions(8, 8, 1));

    a(1, 1, 0) = 5;
    b(1, 1, 0) = 3;

    BinExpr<SumOp, FieldExpr, FieldExpr> typedef AB;
    BinExpr<SumOp, AB, FieldExpr> typedef ABC;

    d <<= ABC(AB(FieldExpr(a), FieldExpr(b)), FieldExpr(c));

    std::cout << d;
    return 0;
}
