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


struct SingleValue;

// JoinLocation metafunction

template<typename Location1, typename Location2>
struct JoinLocation;

template<typename Location>
struct JoinLocation<Location, Location> {
    Location typedef result;
};

template<>
struct JoinLocation<SingleValue, SingleValue> {
    SingleValue typedef result;
};

template<typename OtherLocation>
struct JoinLocation<SingleValue, OtherLocation> {
    OtherLocation typedef result;
};

template<typename OtherLocation>
struct JoinLocation<OtherLocation, SingleValue> {
    OtherLocation typedef result;
};

// CheckEqual metafunction

template<typename Type1, typename Type2>
struct CheckEqual;

template<typename Type>
struct CheckEqual<Type, Type> {
    Type typedef result;
};

// Specific locations

struct SVol;

// Constant expressions

struct ConstExpr {
    SingleValue typedef location;
    double typedef value_type;

    const double value;

    ConstExpr(const double valueArg) : value(valueArg) {}

    double eval(int x, int y, int z, const Grid fieldDim) const {
        return value;
    }
};


// Fields

template<typename Location, typename ValueType>
class Field {
    private:
        double* array;

    public:
        Location typedef location;
        ValueType typedef value_type;

        const Grid dim;

        Field(const Grid dimArg)
            : dim(dimArg)
        {
            array = new double[dim.x * dim.y * dim.z];
        }

        ~Field() {
            delete array;
        }

        double & operator() (int x, int y, int z) {
            return array[z * dim.y + y * dim.x + x];
        }

        double operator() (int x, int y, int z) const {
            return array[z * dim.y + y * dim.x + x];
        }

        double eval(int x, int y, int z, const Grid fieldDim) const {
            return (*this)(x, y, z);
        }

};

// Field reference

template<typename FieldType>
class FieldRef {
    private:
        const FieldType & field;

    public:
        typename FieldType::location typedef location;
        typename FieldType::value_type typedef value_type;

        FieldRef(const FieldType & fieldArg) : field(fieldArg) {}

        double eval(int x, int y, int z, const Grid fieldDim) const {
            return field.eval(x, y, z, fieldDim);
        }
};

// Printing Fields to std::cout

template<typename Location, typename ValueType>
std::ostream & operator<<(std::ostream & os, const Field<Location, ValueType> & field)
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

template<typename Location, typename ValueType, typename Expr>
void operator<<=(Field<Location, ValueType> & field, const Expr & rhs) {
    typename CheckEqual<typename JoinLocation<Location,
                                              typename Expr::location>::result,
                        Location>::result typedef location_check;
    typename CheckEqual<ValueType, typename Expr::value_type>::result typedef value_type_check;

    for (int k = 0; k <  field.dim.z; k++) {
        for (int j = 0; j < field.dim.y; j++) {
            for (int i = 0; i < field.dim.x; i++) {
                field(i, j, k) = rhs.eval(i, j, k, field.dim);
            }
        }
    }
}

template<typename Location, typename ValueType>
void operator<<=(Field<Location, ValueType> & field, const double rhs) {
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
        typename JoinLocation<typename SubExpr1::location, typename SubExpr2::location>::result typedef location;
        typename CheckEqual<typename SubExpr1::value_type, typename SubExpr2::value_type>::result typedef value_type;

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

template<typename Location, typename ValueType>
const FieldRef<Field<Location, ValueType> > wrap(const Field<Location, ValueType> & arg) {
    return FieldRef<Field<Location, ValueType> >(arg);
}

template<typename Expr>
const Expr & wrap(const Expr & arg) {
    return arg;
}


// Wrap return type metafunction

template<typename Expr>
struct WrapReturn {
    Expr typedef result;
};

template<>
struct WrapReturn<double> {
    ConstExpr typedef result;
};

template<typename Location, typename ValueType>
struct WrapReturn<Field<Location, ValueType> > {
    FieldRef<Field<Location, ValueType> > typedef result;
};

template<>
struct WrapReturn<int> {
    ConstExpr typedef result;
};


// Particular operators

struct SumOp {
    double operator() (double lhs, double rhs) const {
        return lhs + rhs;
    }
};

template<typename Arg1, typename Arg2>
BinExpr<SumOp, typename WrapReturn<Arg1>::result, typename WrapReturn<Arg2>::result> operator+(const Arg1 & arg1, const Arg2 & arg2) {
    return BinExpr<SumOp, typename WrapReturn<Arg1>::result, typename WrapReturn<Arg2>::result>(wrap(arg1), wrap(arg2));
}

struct MultOp {
    double operator() (double lhs, double rhs) const {
        return lhs * rhs;
    }
};

template<typename Arg1, typename Arg2>
BinExpr<MultOp, typename WrapReturn<Arg1>::result, typename WrapReturn<Arg2>::result> operator*(const Arg1 & arg1, const Arg2 & arg2) {
    return BinExpr<MultOp, typename WrapReturn<Arg1>::result, typename WrapReturn<Arg2>::result>(wrap(arg1), wrap(arg2));
}
