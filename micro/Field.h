// Dimensions

struct Dimensions {
    const int x;
    const int y;
    const int z;

    Dimensions(int xdim, int ydim, int zdim)
        : x(xdim), y(ydim), z(zdim) {}
};


// Field

class Field {
    private:
        double* cpu_array;

    public:
        const Dimensions dim;

        Field(const Dimensions dimArg)
            : dim(dimArg)
        {
            cpu_array = new double[dim.x * dim.y * dim.z];
        }

        ~Field() {
            delete cpu_array;
        }

        double & operator()(int x, int y, int z) {
            return cpu_array[z * dim.y + y * dim.x + x];
        }

        double operator()(int x, int y, int z) const {
            return cpu_array[z * dim.y + y * dim.x + x];
        }
};
