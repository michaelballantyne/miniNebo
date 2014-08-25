#include "Untyped.h"

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

    a <<= 5;
    a <<= 5.0;
    a <<= (a + 5) * 2;
    a <<= (a + 5.0) * 2;

    #ifdef __CUDACC__
    a.copy_gpu_to_cpu();
    #endif

    std::cout << a;
    return 0;
}
