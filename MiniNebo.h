#include<cfloat>
#include <iostream>

// Grid

struct Grid {
    const int x;
    const int y;
    const int z;

    const double length;

    Grid(int xdim, int ydim, int zdim, double lengthArg)
        : x(xdim), y(ydim), z(zdim), length(lengthArg) {}

    double x_spacing() const {
        return length / x;
    }

    double y_spacing() const {
        return length / y;
    }

    double z_spacing() const {
        return length / z;
    }
};


// Generic field type magic

template<typename ValueType>
class GenericFieldType {
    typedef ValueType value_type;
};

// Need something that only compiles if value types match, but returns the (non-generic) field type
template<typename ValueType1, typename ValueType2, typename FieldType>
struct ValueTypeCheck;

template<typename ValueType, typename FieldType>
struct ValueTypeCheck<ValueType, ValueType, FieldType> {
    FieldType typedef Result;
};

template<typename FieldType1, typename FieldType2>
struct RefineFieldType;

// Covers Generic + Generic (if matching value_types) and Type + Type
template<typename FieldType>
struct RefineFieldType<FieldType, FieldType>
{
    FieldType typedef Result;
};

template<typename ValueType>
struct RefineFieldType<GenericFieldType<ValueType>, GenericFieldType<ValueType> > {
    GenericFieldType<ValueType> typedef Result;
};

template<typename FieldType, typename ValueType>
struct RefineFieldType<GenericFieldType<ValueType>, FieldType> {
    typename ValueTypeCheck<ValueType, typename FieldType::value_type, FieldType>::Result typedef Result;
};

template<typename FieldType, typename ValueType>
struct RefineFieldType<FieldType, GenericFieldType<ValueType> > {
    typename ValueTypeCheck<ValueType, typename FieldType::value_type, FieldType>::Result typedef Result;
};

template<typename SubExpr, typename FieldType>
struct ExprTypeCheck {
    typename RefineFieldType<typename SubExpr::field_type, FieldType>::Result typedef Result;
};


// Specific types

struct SVol {
    double typedef value_type;
};


// Constant expressions

struct ConstExpr {
    GenericFieldType<double> typedef field_type;

    const double value;

    ConstExpr(const double valueArg) : value(valueArg) {}

    double eval(int x, int y, int z, const Grid fieldDim) const {
        return value;
    }
};


// Fields

template<typename FieldType>
class Field {
    private:
        struct FieldMemory {
            double* array;

            FieldMemory(const int x, const int y, const int z) {
                array = new double[x * y * z];
            }

            ~FieldMemory() {
                delete array;
            }
        };

        std::shared_ptr<FieldMemory> data;

    public:
        FieldType typedef field_type;

        const Grid dim;

        Field(const Grid dimArg)
            : dim(dimArg), data(new FieldMemory(dimArg.x, dimArg.y, dimArg.z)) {}

        double & operator() (int x, int y, int z) {
            return (*data).array[z * dim.y + y * dim.x + x];
        }

        double operator() (int x, int y, int z) const {
            return (*data).array[z * dim.y + y * dim.x + x];
        }

        double eval(int x, int y, int z, const Grid fieldDim) const {
            return (*this)(x, y, z);
        }

};


// Printing Fields to std::cout

template<typename FieldType>
std::ostream & operator<<(std::ostream & os, const Field<FieldType> & field)
{
    for (int z = 0; z < field.dim.z; z++) {
        os << "z = " << z << std::endl;
        for (int y = 0; y < field.dim.y; y++) {
            for (int x = 0; x < field.dim.x; x++) {
                os << field(x, y, z) << " ";
            }
            os << std::endl;
        }
        os << std::endl;
    }

    return os;
}


// Assignments

template<typename FieldType, typename Expr>
void operator<<=(Field<FieldType> & field, const Expr & rhs) {
    typename ExprTypeCheck<Expr, FieldType>::Result typedef check;

    for (int k = 0; k <  field.dim.z; k++) {
        for (int j = 0; j < field.dim.y; j++) {
            for (int i = 0; i < field.dim.x; i++) {
                field(i, j, k) = rhs.eval(i, j, k, field.dim);
            }
        }
    }
}

template<typename FieldType>
void operator<<=(Field<FieldType> & field, const double rhs) {
    field <<= ConstExpr(rhs);
}


// Binary expressions

template<typename Functor, typename SubExpr1, typename SubExpr2>
class BinExpr {
    private:
        const SubExpr1 subExpr1;
        const SubExpr2 subExpr2;
        const Functor functor;

    public:
        typename RefineFieldType<typename SubExpr1::field_type, typename SubExpr2::field_type>::Result typedef field_type;

        BinExpr(const SubExpr1 & subExpr1Arg, const SubExpr2 & subExpr2Arg)
            : subExpr1(subExpr1Arg), subExpr2(subExpr2Arg), functor() {}

        double eval(int x, int y, int z, const Grid fieldDim) const {
            return functor(subExpr1.eval(x, y, z, fieldDim), subExpr2.eval(x, y, z, fieldDim));
        }
};


// Wrap function

const ConstExpr wrap(double valueArg) {
    return ConstExpr(valueArg);
}

const ConstExpr wrap(int valueArg) {
    return ConstExpr(valueArg);
}

template<typename Expr>
const Expr & wrap(const Expr & arg) {
    return arg;
}


// Wrap return type metafunction

template<typename Expr>
struct WrapReturn {
    Expr typedef Result;
};

template<>
struct WrapReturn<double> {
    ConstExpr typedef Result;
};

template<>
struct WrapReturn<int> {
    ConstExpr typedef Result;
};


// Particular operators

struct SumOp {
    double operator() (double lhs, double rhs) const {
        return lhs + rhs;
    }
};

template<typename Arg1, typename Arg2>
BinExpr<SumOp, typename WrapReturn<Arg1>::Result, typename WrapReturn<Arg2>::Result> operator+(const Arg1 & arg1, const Arg2 & arg2) {
    return BinExpr<SumOp, typename WrapReturn<Arg1>::Result, typename WrapReturn<Arg2>::Result>(wrap(arg1), wrap(arg2));
}

struct MultOp {
    double operator() (double lhs, double rhs) const {
        return lhs * rhs;
    }
};

template<typename Arg1, typename Arg2>
BinExpr<MultOp, typename WrapReturn<Arg1>::Result, typename WrapReturn<Arg2>::Result> operator*(const Arg1 & arg1, const Arg2 & arg2) {
    return BinExpr<MultOp, typename WrapReturn<Arg1>::Result, typename WrapReturn<Arg2>::Result>(wrap(arg1), wrap(arg2));
}
