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


// Specific reductions

struct Min {
    static const double initial;

    double operator() (double el, double acc) {
        return el < acc ? el : acc;
    }
};

const double Min::initial = DBL_MAX;
