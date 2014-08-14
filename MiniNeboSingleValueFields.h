template<typename ValueType>
class Field<SingleValue, ValueType> {
    private:
        double value;

    public:
        const Grid dim;

        ValueType typedef value_type;
        SingleValue typedef location;

        Field() : value(0), dim(1, 1, 1, 1) {}
        Field(double valueArg) : value(valueArg), dim(1, 1, 1, 1) {}

        double & operator() (int x, int y, int z) {
            return value;
        }

        double operator() (int x, int y, int z) const {
            return value;
        }

        double eval(int x, int y, int z, const Grid fieldDim) const {
            return (*this)(x, y, z);
        }
};

Field<SingleValue, double> typedef SingleValueField;
