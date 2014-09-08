// Dimensions

struct Dimensions {
    const int x;
    const int y;
    const int z;

    Dimensions(int xdim, int ydim, int zdim)
        : x(xdim), y(ydim), z(zdim) {}
};


// Field

template<typename Location>
class TypedField {
    private:
        double* cpu_array;

    public:
        Location typedef location;

        const Dimensions dim;

        TypedField(const Dimensions dimArg)
            : dim(dimArg)
        {
            cpu_array = new double[dim.x * dim.y * dim.z];
        }

        ~TypedField() {
            delete cpu_array;
        }

        double* get_data() const {
            return cpu_array;
        }
};

struct Untyped;
TypedField<Untyped> typedef Field;
