#include "Field.h"


int main() {
    Dimensions dim(128, 128, 1);

    Field a(dim);
    Field b(dim);

    double* aa = a.get_data();
    double* ab = b.get_data();

    for (int it = 0; it < 1000000; it++) {
        for (int i = 0; i < dim.z * dim.y * dim.x; i++) {
            aa[i] = aa[i] + (ab[i] * 5.1);
        }
    }
}
