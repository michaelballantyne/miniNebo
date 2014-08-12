template<typename ValueType>
class Field<GenericFieldType<ValueType> > {
    private:
        double value;

    public:
        const Grid dim;

        GenericFieldType<ValueType> typedef field_type;

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

Field<GenericFieldType<double> > typedef SingleValueField;
