#include "Untyped.h"

#include<iostream>

int main(int argc, const char *argv[])
{
    Field a(Dimensions(8, 8, 1));

    a <<= 5;
    a <<= 5.0;
    a <<= (a + 5) * 2;
    a <<= (a + 5.0) * 2;

    std::cout << a;
    return 0;
}
