#include<cmath>
#include<iostream>

#include "MiniNebo.h"


struct InitialAlpha {
    SVol typedef field_type;

    double eval(int x, int y, int z, const Grid fieldDim) const {
        return 1.0;
    }
};


template<typename SubExpr>
struct PhiBoundaryConditions {
    SVol typedef field_type;

    const SubExpr & internalExpr;

    PhiBoundaryConditions(const SubExpr & internalExprArg)
    : internalExpr(internalExprArg) {}

    double eval(int x, int y, int z, const Grid fieldDim) const {
        if (x == 0) {
            return 10.0;
        }

        if (x == fieldDim.x - 1) {
            return 0.0;
        }

        if (y == 0 || y == fieldDim.y - 1) {
            return 5.0;
        }

        return internalExpr.eval(x, y, z, fieldDim);
    }
};

template<typename SubExpr>
PhiBoundaryConditions<SubExpr> phiBoundaryConditions(const SubExpr & internalExprArg) {
    return PhiBoundaryConditions<SubExpr>(internalExprArg);
}


double find_deltaT(const Field<SVol> & alpha, const Grid & grid) {
    double sqrdDeltaX = pow(grid.x_spacing(), 2);
    double sqrdDeltaY = pow(grid.y_spacing(), 2);

    return 0.25 * sqrdDeltaX * sqrdDeltaY /
        ((sqrdDeltaX + sqrdDeltaY) * reduce<Min>(alpha));
}

int main(int argc, const char *argv[])
{
    Grid grid(8, 8, 1, 1.3333333333333f);

    Field<SVol> alpha(grid);
    Field<SVol> phi(grid);
    Field<SVol> Q(grid);

    alpha <<= InitialAlpha();

    phi <<= phiBoundaryConditions(constExpr(5.0));

    double deltaT = find_deltaT(alpha, grid);

    std::cout << "alpha:" << std::endl;
    std::cout << alpha;
    std::cout << "x spacing " << grid.x_spacing() << std::endl;
    std::cout << "y spacing " << grid.y_spacing() << std::endl;
    std::cout << "deltaT " << deltaT << std::endl;
    std::cout << std::endl;
    std::cout << "Initial phi:" << std::endl;
    std::cout << phi;

    const int time_steps = 40;

    for (int step = 1; step <= time_steps; step++) {
        Q <<= sum(divX(mult(interpX(alpha), gradX(phi))),
                  divY(mult(interpY(alpha), gradY(phi))));

        phi <<= sum(phi, mult(constExpr(deltaT), Q));

        phi <<= phiBoundaryConditions(phi);

        std::cout << "phi after " << step << " time steps:" << std::endl;
        std::cout << phi;
    }

    return 0;
}
