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
        typename SubExpr::value_type typedef value_type;
        SSurfX typedef location;

        GradX(const SubExpr & subExprArg) : subExpr(subExprArg) {}

        value_type eval(int x, int y, int z, const Grid fieldDim) const {
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
    typename CheckEqual<typename SubExpr::location, SVol>::result typedef check;

    return GradX<SubExpr>(subExprArg);
}

template<typename SubExpr>
class DivX {
    private:
        const SubExpr & subExpr;

    public:
        typename SubExpr::value_type typedef value_type;
        SVol typedef location;

        DivX(const SubExpr & subExprArg) : subExpr(subExprArg) {}

        value_type eval(int x, int y, int z, const Grid fieldDim) const {
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
    typename CheckEqual<typename SubExpr::location, SSurfX>::result typedef check;

    return DivX<SubExpr>(subExprArg);
}

template<typename SubExpr>
class InterpX {
    private:
        const SubExpr & subExpr;

    public:
        typename SubExpr::value_type typedef value_type;
        SSurfX typedef location;

        InterpX(const SubExpr & subExprArg) : subExpr(subExprArg) {}

        value_type eval(int x, int y, int z, const Grid fieldDim) const {
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
    typename CheckEqual<typename SubExpr::location, SVol>::result typedef check;

    return InterpX<SubExpr>(subExprArg);
}


// Y

template<typename SubExpr>
class GradY {
    private:
        const SubExpr & subExpr;

    public:
        typename SubExpr::value_type typedef value_type;
        SSurfY typedef location;

        GradY(const SubExpr & subExprArg) : subExpr(subExprArg) {}

        value_type eval(int x, int y, int z, const Grid fieldDim) const {
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
    typename CheckEqual<typename SubExpr::location, SVol>::result typedef check;

    return GradY<SubExpr>(subExprArg);
}

template<typename SubExpr>
class DivY {
    private:
        const SubExpr & subExpr;

    public:
        typename SubExpr::value_type typedef value_type;
        SVol typedef location;

        DivY(const SubExpr & subExprArg) : subExpr(subExprArg) {}

        value_type eval(int x, int y, int z, const Grid fieldDim) const {
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
    typename CheckEqual<typename SubExpr::location, SSurfY>::result typedef check;

    return DivY<SubExpr>(subExprArg);
}

template<typename SubExpr>
class InterpY {
    private:
        const SubExpr & subExpr;

    public:
        typename SubExpr::value_type typedef value_type;
        SSurfY typedef location;

        InterpY(const SubExpr & subExprArg) : subExpr(subExprArg) {}

        value_type eval(int x, int y, int z, const Grid fieldDim) const {
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
    typename CheckEqual<typename SubExpr::location, SVol>::result typedef check;

    return InterpY<SubExpr>(subExprArg);
}

