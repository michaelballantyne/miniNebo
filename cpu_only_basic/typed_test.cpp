#include "Typed.h"

#include<iostream>

int main(int argc, const char *argv[])
{
    TypedField<SVol> a(Dimensions(8, 8, 1));

    a <<= 5;
    a <<= 5.0;
    a <<= (a + 5) * 2;
    a <<= (a + 5.0) * 2;

    std::cout << a;

    // This should fail, as Untyped and SVol shouldn't be compatible.
    //TypedField<Untyped> d(Dimensions(8, 8, 1), device);
    //a <<= d;

    return 0;
}
