#include "Typed.h"

#include<iostream>

int main(int argc, const char *argv[])
{
    TypedField<SVol> a(Dimensions(128, 128, 1));
    TypedField<SVol> b(Dimensions(128, 128, 1));

    // Vectorized
    a <<= 5.0;

    // This won't be vectorized, but doesn't hurt others.
    //a <<= a + 5.0;

    for (int i = 0; i < 100000; i++) {
        // Vectorized.
        a <<= b + (b * 5.1);
    }

    // This breaks vectorization of everything else
    //std::cout << a;

    // This should fail, as Untyped and SVol shouldn't be compatible.
    //TypedField<Untyped> d(Dimensions(8, 8, 1), device);
    //a <<= d;

    return 0;
}
