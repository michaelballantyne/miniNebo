#include<iostream>

#include "MiniNebo.h"
#include "MiniNeboSingleValueFields.h"

int main(int argc, const char *argv[])
{
    Grid grid(8, 8, 1, 1.3333333);

    SingleValueField a(1.0);

    SingleValueField b;
    b <<= a + 1;
    std::cout << b;

    Field<SVol, double> c(grid);
    c <<= -2.0;

    Field<SVol, double> d(grid);
    d <<= c + b + 1;
    std::cout << d;

    double e = 5.5 + 1.4;

    // This should fail to compile; the assignment would lose information.
    // b <<= c;

    return 0;
}
