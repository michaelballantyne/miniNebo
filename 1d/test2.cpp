#include <iostream>
#include "Core.h"

int main(int argc, const char *argv[])
{
    Field a(2);

    a(0) = 5;

    std::cout << FieldExpr(a).eval(0);

    return 0;
}
