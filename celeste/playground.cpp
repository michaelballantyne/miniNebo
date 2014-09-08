#include<iostream>
#include<vector>

std::vector<int> operator+(std::vector<int> lhs, std::vector<int> rhs) {
    std::vector<int> returnValue(lhs.size());

    for (int i = 0; i < lhs.size(); i++) {
        returnValue[i] = lhs[i] + rhs[i];
    }

    return returnValue;
}

int main(int argc, const char *argv[])
{
    std::vector<int> ints1(2);
    std::vector<int> ints2(2);
    ints1[0] = 1;
    ints1[1] = 2;
    ints2[0] = 3;
    ints2[1] = 4;

    std::vector<int> ints3(2);
    ints3 = ints1 + ints2;

    std::cout << ints3[0] << " should be 4" << std::endl;
    std::cout << ints3[1] << " should be 6" << std::endl;

    return 0;
}
