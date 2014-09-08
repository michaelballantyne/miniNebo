#include "Core.h"

#include<iostream>

int main(int argc, const char *argv[])
{
    Field a(Dimensions(8, 8, 1));
    ConstExpr b(5), c(2);

    core_assign(a, b);

    FieldExpr<Field> typedef A;
    BinExpr<SumOp, A, ConstExpr> typedef AB;
    BinExpr<MultOp, AB, ConstExpr> typedef ABC;

    core_assign(a, ABC(AB(A(a), b), c));

    core_write_field(std::cout, a);
    return 0;
}
