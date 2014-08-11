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

struct SVol {
    double typedef value_type;
};

struct SSurfX {
    double typedef value_type;
};

struct SSurfY {
    double typedef value_type;
};

template<typename FieldType>
class Field {
    private:
        double* array;

    public:
        FieldType typedef field_type;

        const Grid dim;

        Field(const Grid dimArg)
            : dim(dimArg)
        {
            array = new double[dim.x * dim.y * dim.z];
        }

        ~Field() {
            delete array;
        }

        double & operator() (int x, int y, int z) const {
            return array[z * dim.y + y * dim.x + x];
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

template<typename Functor, typename SubExpr1, typename SubExpr2>
class BinExpr {
    private:
        const SubExpr1 & subExpr1;
        const SubExpr2 & subExpr2;
        const Functor functor;

    public:
        typename RefineFieldType<typename SubExpr1::field_type, typename SubExpr2::field_type>::Result typedef field_type;

        BinExpr(const SubExpr1 & subExpr1Arg, const SubExpr2 & subExpr2Arg)
            : subExpr1(subExpr1Arg), subExpr2(subExpr2Arg), functor() {}

        double eval(int x, int y, int z, const Grid fieldDim) const {
            return functor(subExpr1.eval(x, y, z, fieldDim), subExpr2.eval(x, y, z, fieldDim));
        }
};

struct ConstExpr {
    GenericFieldType<double> typedef field_type;

    double value;

    ConstExpr(double valueArg) : value(valueArg) {}

    double eval(int x, int y, int z, const Grid fieldDim) const {
        return value;
    }
};

ConstExpr constExpr(double valueArg) {
    return ConstExpr(valueArg);
}

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


// Particular operators

struct SumOp {
    double operator() (double lhs, double rhs) const {
        return lhs + rhs;
    }
};

template<typename SubExpr1, typename SubExpr2>
BinExpr<SumOp, SubExpr1, SubExpr2> sum(const SubExpr1 & subExpr1Arg, const SubExpr2 & subExpr2Arg) {
    return BinExpr<SumOp, SubExpr1, SubExpr2>(subExpr1Arg, subExpr2Arg);
}

struct MultOp {
    double operator() (double lhs, double rhs) const {
        return lhs * rhs;
    }
};

template<typename SubExpr1, typename SubExpr2>
BinExpr<MultOp, SubExpr1, SubExpr2> mult(const SubExpr1 & subExpr1Arg, const SubExpr2 & subExpr2Arg) {
    return BinExpr<MultOp, SubExpr1, SubExpr2>(subExpr1Arg, subExpr2Arg);
}

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
