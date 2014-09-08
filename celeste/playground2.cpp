#include<iostream>
#include<vector>

struct Dimensions {
    const int x;
    const int y;
    const int z;

    Dimensions(const int argX, const int argY, const int argZ)
        : x(argX), y(argY), z(argZ) {}
};

struct Vector {
    Dimensions dim;
    int* array;

    Vector(Dimensions dimArg) : dim(dimArg) {
        array = new int[dimArg.z * dimArg.y * dimArg.x];
    }

    int operator()(int x, int y, int z) const {
        return array[z * dim.y * dim.x + y * dim.x + x];
    }

    int & operator()(int x, int y, int z) {
        return array[z * dim.y * dim.x + y * dim.x + x];
    }

    ~Vector() {
        delete array;
    }
};


template<typename SubExpr1, typename SubExpr2>
class SumExpr {
    private:
        const SubExpr1 & lhs;
        const SubExpr2 & rhs;

    public:
        SumExpr(const SubExpr1 & lhsArg, const SubExpr2 & rhsArg)
         : lhs(lhsArg), rhs(rhsArg) {}

        int operator()(int x, int y, int z) const {
            return lhs(x, y, z) + rhs(x, y, z);
        }
};

template<typename LHS, typename RHS>
SumExpr<LHS, RHS> operator+(const LHS & lhs, const RHS & rhs) {
    return SumExpr<LHS, RHS>(lhs, rhs);
}

template<typename RHS>
void operator<<=(Vector & lhs, const RHS & rhs) {
    for (int z = 0; z < lhs.dim.z; z++) {
        for (int y = 0; y < lhs.dim.y; y++) {
            for (int x = 0; x < lhs.dim.x; x++) {
                lhs(x, y, z) = rhs(x, y, z);
            }
        }
    }
}

int main(int argc, const char *argv[])
{
    Dimensions dim(8, 8, 1);
    Vector ints1(dim);
    Vector ints2(dim);
    Vector ints3(dim);
    ints1(0, 0, 0) = 1;
    ints1(1, 0, 0) = 2;
    ints2(0, 0, 0) = 3;
    ints2(1, 0, 0) = 4;
    ints3(0, 0, 0) = 3;
    ints3(1, 0, 0) = 4;

    Vector ints4(dim);
    ints4 <<= ints1 + ints2 + ints3;

    //for (int i = 0; i < ints4.size; i++) {
        //ints4[i] = ints1[i] + ints2[i] + ints3[i];
    //}

    std::cout << ints4(0, 0, 0);
    std::cout << ints4(1, 0, 0);

    return 0;
}
