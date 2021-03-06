#include<cmath>
#include<iostream>

#include "MiniNebo.h"
#include "MiniNeboStencils.h"
#include "MiniNeboReductions.h"


struct InitialAlpha {
    SVol typedef location;
    double typedef value_type;

    double eval(int x, int y, int z, const Grid fieldDim) const {
        return 1.0;
    }
};


template<typename SubExpr>
struct PhiBoundaryConditions {
    SVol typedef location;
    double typedef value_type;

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


double find_deltaT(const Field<SVol, double> & alpha, const Grid & grid) {
    double sqrdDeltaX = pow(grid.x_spacing(), 2);
    double sqrdDeltaY = pow(grid.y_spacing(), 2);

    return 0.25 * sqrdDeltaX * sqrdDeltaY /
        ((sqrdDeltaX + sqrdDeltaY) * reduce<Min>(alpha));
}

int main(int argc, const char *argv[])
{
    Grid grid(8, 8, 1, 1.3333333333333f);

    Field<SVol, double> alpha(grid);
    Field<SVol, double> phi(grid);
    Field<SVol, double> Q(grid);

    alpha <<= InitialAlpha();

    phi <<= phiBoundaryConditions(wrap(5.0));

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
        Q <<= divX(interpX(alpha) * gradX(phi)) +
              divY(interpY(alpha) * gradY(phi));

        phi <<= phi + deltaT * Q;

        phi <<= phiBoundaryConditions(phi);

        std::cout << "phi after " << step << " time steps:" << std::endl;
        std::cout << phi;
    }

    return 0;
}
