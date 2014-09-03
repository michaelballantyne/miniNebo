#include "Core.h"

#include<iostream>
#include<vector>

int main(int argc, const char *argv[])
{
    std::vector<double> a(8);
    std::vector<double> b(8);
    std::vector<double> c(8);
    std::vector<double> d(8);

    a[1] = 5;
    b[1] = 3;

    BinExpr<SumOp, VectorExpr, VectorExpr> typedef AB;
    BinExpr<SumOp, AB, VectorExpr> typedef ABC;

    d <<= ABC(AB(VectorExpr(a), VectorExpr(b)), VectorExpr(c));

    std::cout << d;
    return 0;
}
