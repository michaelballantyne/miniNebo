#include <iostream>
#include "Core.h"

int main(int argc, const char *argv[])
{
    Field a(10);
    Field b(10);

    a(0) = 5;
    b(0) = 7;

    std::cout << BinExpr<SumOp, FieldExpr, FieldExpr>(FieldExpr(a), FieldExpr(b)).eval(0);

    return 0;
}
