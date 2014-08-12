#include<cfloat>
#include <iostream>

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

struct SSurfX {
    double typedef value_type;
};

struct SSurfY {
    double typedef value_type;
};


// Field

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

        double & operator() (int x, int y, int z) const {
            return (*data).array[z * dim.y + y * dim.x + x];
        }

        double eval(int x, int y, int z, const Grid fieldDim) const {
            return (*this)(x, y, z);
        }

        template<typename Expr>
        void operator<<= (const Expr & rhs) {
            typename ExprTypeCheck<Expr, FieldType>::Result typedef check;

            for (int k = 0; k < dim.z; k++) {
                for (int j = 0; j < dim.y; j++) {
                    for (int i = 0; i < dim.x; i++) {
                        (*this)(i, j, k) = rhs.eval(i, j, k, dim);
                    }
                }
            }
        }
};


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


// Constant expressions

struct ConstExpr {
    GenericFieldType<double> typedef field_type;

    const double value;

    ConstExpr(const double valueArg) : value(valueArg) {}

    double eval(int x, int y, int z, const Grid fieldDim) const {
        return value;
    }
};

ConstExpr constExpr(double valueArg) {
    return ConstExpr(valueArg);
}

// Reduction function

template<typename Reduction, typename FieldType>
double reduce(const Field<FieldType> & field) {
    Reduction reduction;

    double acc = reduction.initial;

    for (int i = 0; i < field.dim.x; i++) {
        for (int j = 0; j < field.dim.y; j++) {
            for (int k = 0; k < field.dim.z; k++) {
                acc = reduction(field(i, j, k), acc);
            }
        }
    }

    return acc;
}

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


// binExpr function specializations

template<typename Op, typename SubExpr1, typename SubExpr2>
BinExpr<Op, SubExpr1, SubExpr2> binExpr(const SubExpr1 & subExpr1Arg, const SubExpr2 & subExpr2Arg) {
    return BinExpr<Op, SubExpr1, SubExpr2>(subExpr1Arg, subExpr2Arg);
}

template<typename Op, typename SubExpr>
BinExpr<Op, SubExpr, ConstExpr> binExpr(const SubExpr & subExprArg, double scalarArg) {
    return BinExpr<Op, SubExpr, ConstExpr>(subExprArg, ConstExpr(scalarArg));
}

template<typename Op, typename SubExpr>
BinExpr<Op, ConstExpr, SubExpr> binExpr(double scalarArg, const SubExpr & subExprArg) {
    return BinExpr<Op, ConstExpr, SubExpr>(ConstExpr(scalarArg), subExprArg);
}

template<typename Op, typename SubExpr>
BinExpr<Op, SubExpr, ConstExpr> binExpr(const SubExpr & subExprArg, int scalarArg) {
    return BinExpr<Op, SubExpr, ConstExpr>(subExprArg, ConstExpr(scalarArg));
}

template<typename Op, typename SubExpr>
BinExpr<Op, ConstExpr, SubExpr> binExpr(int scalarArg, const SubExpr & subExprArg) {
    return BinExpr<Op, ConstExpr, SubExpr>(ConstExpr(scalarArg), subExprArg);
}


// binExpr return type metafunction

template<typename Op, typename SubExpr1, typename SubExpr2>
struct BinExprReturn {
    BinExpr<Op, SubExpr1, SubExpr2> typedef Result;
};

template<typename Op, typename SubExpr>
struct BinExprReturn<Op, SubExpr, double> {
    BinExpr<Op, SubExpr, ConstExpr> typedef Result;
};

template<typename Op, typename SubExpr>
struct BinExprReturn<Op, double, SubExpr> {
    BinExpr<Op, ConstExpr, SubExpr> typedef Result;
};

template<typename Op, typename SubExpr>
struct BinExprReturn<Op, SubExpr, int> {
    BinExpr<Op, SubExpr, ConstExpr> typedef Result;
};

template<typename Op, typename SubExpr>
struct BinExprReturn<Op, int, SubExpr> {
    BinExpr<Op, ConstExpr, SubExpr> typedef Result;
};


// Particular operators

// Binary

struct SumOp {
    double operator() (double lhs, double rhs) const {
        return lhs + rhs;
    }
};

template<typename Arg1, typename Arg2>
typename BinExprReturn<SumOp, Arg1, Arg2>::Result operator+(const Arg1 & arg1, const Arg2 & arg2) {
    return binExpr<SumOp>(arg1, arg2);
}

struct MultOp {
    double operator() (double lhs, double rhs) const {
        return lhs * rhs;
    }
};

template<typename Arg1, typename Arg2>
typename BinExprReturn<MultOp, Arg1, Arg2>::Result operator*(const Arg1 & arg1, const Arg2 & arg2) {
    return binExpr<MultOp>(arg1, arg2);
}

// Reduction

struct Min {
    static const double initial;

    double operator() (double el, double acc) {
        return el < acc ? el : acc;
    }
};

const double Min::initial = DBL_MAX;



// Stencils

// X

template<typename SubExpr>
class GradX {
    private:
        const SubExpr & subExpr;

    public:
        SSurfX typedef field_type;

        GradX(const SubExpr & subExprArg) : subExpr(subExprArg) {}

        double eval(int x, int y, int z, const Grid fieldDim) const {
            if (x == 0) {
                return 0;
            } else {
                return -subExpr.eval(x - 1, y, z, fieldDim) / fieldDim.x_spacing() +
                        subExpr.eval(x    , y, z, fieldDim) / fieldDim.x_spacing();
            }
        }
};

template<typename SubExpr>
GradX<SubExpr> gradX(const SubExpr & subExprArg) {
    typename ExprTypeCheck<SubExpr, SVol>::Result typedef check;

    return GradX<SubExpr>(subExprArg);
}

template<typename SubExpr>
class DivX {
    private:
        const SubExpr & subExpr;

    public:
        SVol typedef field_type;

        DivX(const SubExpr & subExprArg) : subExpr(subExprArg) {}

        double eval(int x, int y, int z, const Grid fieldDim) const {
            if (x == fieldDim.x - 1) {
                return 0;
            } else {
                return -subExpr.eval(x    , y, z, fieldDim) / fieldDim.x_spacing() +
                        subExpr.eval(x + 1, y, z, fieldDim) / fieldDim.x_spacing();
            }
        }
};

template<typename SubExpr>
DivX<SubExpr> divX(const SubExpr & subExprArg) {
    typename ExprTypeCheck<SubExpr, SSurfX>::Result typedef check;

    return DivX<SubExpr>(subExprArg);
}

template<typename SubExpr>
class InterpX {
    private:
        const SubExpr & subExpr;

    public:
        SSurfX typedef field_type;

        InterpX(const SubExpr & subExprArg) : subExpr(subExprArg) {}

        double eval(int x, int y, int z, const Grid fieldDim) const {
            if (x == 0) {
                return 0;
            } else {
                return subExpr.eval(x - 1, y, z, fieldDim) / 2 +
                       subExpr.eval(x    , y, z, fieldDim) / 2;
            }
        }
};

template<typename SubExpr>
InterpX<SubExpr> interpX(const SubExpr & subExprArg) {
    typename ExprTypeCheck<SubExpr, SVol>::Result typedef check;

    return InterpX<SubExpr>(subExprArg);
}


// Y

template<typename SubExpr>
class GradY {
    private:
        const SubExpr & subExpr;

    public:
        SSurfY typedef field_type;

        GradY(const SubExpr & subExprArg) : subExpr(subExprArg) {}

        double eval(int x, int y, int z, const Grid fieldDim) const {
            if (y == 0) {
                return 0;
            } else {
                return -subExpr.eval(x, y - 1, z, fieldDim) / fieldDim.y_spacing() +
                        subExpr.eval(x, y    , z, fieldDim) / fieldDim.y_spacing();
            }
        }
};

template<typename SubExpr>
GradY<SubExpr> gradY(const SubExpr & subExprArg) {
    typename ExprTypeCheck<SubExpr, SVol>::Result typedef check;

    return GradY<SubExpr>(subExprArg);
}

template<typename SubExpr>
class DivY {
    private:
        const SubExpr & subExpr;

    public:
        SVol typedef field_type;

        DivY(const SubExpr & subExprArg) : subExpr(subExprArg) {}

        double eval(int x, int y, int z, const Grid fieldDim) const {
            if (y == fieldDim.y - 1) {
                return 0;
            } else {
                return -subExpr.eval(x, y    , z, fieldDim) / fieldDim.y_spacing() +
                        subExpr.eval(x, y + 1, z, fieldDim) / fieldDim.y_spacing();
            }
        }
};

template<typename SubExpr>
DivY<SubExpr> divY(const SubExpr & subExprArg) {
    typename ExprTypeCheck<SubExpr, SSurfY>::Result typedef check;

    return DivY<SubExpr>(subExprArg);
}

template<typename SubExpr>
class InterpY {
    private:
        const SubExpr & subExpr;

    public:
        SSurfY typedef field_type;

        InterpY(const SubExpr & subExprArg) : subExpr(subExprArg) {}

        double eval(int x, int y, int z, const Grid fieldDim) const {
            if (y == 0) {
                return 0;
            } else {
                return subExpr.eval(x, y - 1, z, fieldDim) / 2 +
                       subExpr.eval(x, y    , z, fieldDim) / 2;
            }
        }
};

template<typename SubExpr>
InterpY<SubExpr> interpY(const SubExpr & subExprArg) {
    typename ExprTypeCheck<SubExpr, SVol>::Result typedef check;

    return InterpY<SubExpr>(subExprArg);
}

