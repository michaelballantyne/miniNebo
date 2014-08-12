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

    Field<SVol> c(grid);
    c <<= -2.0;

    Field<SVol> d(grid);
    d <<= c + b + 1;
    std::cout << d;

    return 0;
}
