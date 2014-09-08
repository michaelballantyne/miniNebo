#include "Field.h"


// Comparison vs test.cpp; icpc seems to do better with one loop nesting than a doubly nested loop here.
int main() {
    Dimensions dim(128, 128, 1);

    Field a(dim);
    Field b(dim);

    double* aa = a.get_data();
    double* ab = b.get_data();

    for (int it = 0; it < 1000000; it++) {
        for (int y = 0; y < dim.y; y++) {
            for (int x = 0; x < dim.x; x++) {
                aa[y * dim.x + x] = aa[y * dim.x + x] + (ab[y * dim.x + x] * 5);
            }
        }
    }
}
