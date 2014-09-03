// Dimensions

// Field

class Field {
    private:
        int* cpu_array;

    public:
        const int size;

        Field(const int sizeArg)
            : size(sizeArg)
        {
            cpu_array = new int[size];
        }

        ~Field() {
            delete cpu_array;
        }

        int & operator()(int i) {
            return cpu_array[i];
        }

        int & operator()(int i) const {
            return cpu_array[i];
        }
};
