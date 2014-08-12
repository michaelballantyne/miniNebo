// Types

struct SSurfX {
    double typedef value_type;
};

struct SSurfY {
    double typedef value_type;
};


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

