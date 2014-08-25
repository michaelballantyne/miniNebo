#include "Core.h"

#include<iostream>

#ifdef __CUDACC__
const Device device = DEVICE_GPU;
#else
#define DEVICE DEVICE_CPU
const Device device = DEVICE_CPU;
#endif

int main(int argc, const char *argv[])
{
    Field a(Dimensions(8, 8, 1), device);
    ConstExpr b(5), c(2);

    core_assign(a, b);

    FieldExpr<Initial, Field> typedef A;
    BinExpr<Initial, SumOp, A, ConstExpr> typedef AB;
    BinExpr<Initial, MultOp, AB, ConstExpr> typedef ABC;

    core_assign(a, ABC(AB(A(a), b), c));

    #ifdef __CUDACC__
    a.copy_gpu_to_cpu();
    #endif

    core_write_field(std::cout, a);
    return 0;
}
