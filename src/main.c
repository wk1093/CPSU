#include <stdio.h>

#define CPSU_IMPLEMENTATION
#include <cpsu/cpsu.h>

int main() {
    suVector_i8 *vec = suVectorNew_i8();
    suVectorAdd_i8(vec, 5);

    suVectorFree_i8(vec);
}
