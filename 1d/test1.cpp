#include <iostream>
#include "Field.h"

int main(int argc, const char *argv[])
{
    Field a(2);

    a(0) = 5;

    std::cout << a(0);

    return 0;
}
