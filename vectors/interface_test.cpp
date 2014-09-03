#include "Interface.h"

#include<iostream>

int main(int argc, const char *argv[])
{
    std::vector<double> a(8);
    std::vector<double> b(8);
    std::vector<double> c(8);
    std::vector<double> d(8);

    a[1] = 5;
    b[1] = 3;

    d <<= a + b + c;

    // This should break... I'm not providing every specialization.
    //d <<= a + (b + c);

    // As should this.
    //d <<= (a + b) + (b + c);

    std::cout << d;
    return 0;
}
