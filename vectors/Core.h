#include<cfloat>
#include<iostream>
#include<sstream>
#include<stdexcept>
#include<vector>

// Vector expressions

class VectorExpr {
    private:
        const std::vector<double> & vector;

    public:
        VectorExpr(const std::vector<double> & vectorArg) : vector(vectorArg) {}

        double eval(int i) const {
            return vector[i];
        }
};

// Binary expressions

template<typename Functor, typename SubExpr1, typename SubExpr2>
class BinExpr {
    private:
        const SubExpr1 subExpr1;
        const SubExpr2 subExpr2;
        Functor functor;

    public:
        BinExpr(const SubExpr1 & subExpr1Arg, const SubExpr2 & subExpr2Arg)
            : subExpr1(subExpr1Arg), subExpr2(subExpr2Arg) {}

        double eval(int i) const {
            return functor(subExpr1.eval(i), subExpr2.eval(i));
        }
};

// Operators

struct SumOp {
    double operator() (double lhs, double rhs) const {
        return lhs + rhs;
    }
};


// Assignment

template<typename Expr>
void operator<<=(std::vector<double> & vector, const Expr & expr) {
    for (int i = 0; i < vector.size(); i++) {
        vector[i] = expr.eval(i);
    }
}

// Printing vectors to std::cout

std::ostream & operator<<(std::ostream & os, std::vector<double> & vector)
{
    for (int i = 0; i < vector.size(); i++) {
        os << vector[i] << " ";
    }
    os << std::endl;

    return os;
}
