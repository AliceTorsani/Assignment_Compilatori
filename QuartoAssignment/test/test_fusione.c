#include <stdio.h>

/*TEST Con dei for che non partono da 0 */
void test_fusion(int *A, int *B, int *C, int N) {
    // Loop 0
    for (int i = 1; i < N; i++) {
        A[i] = B[i] * 2;
    }

    // Loop 1
    for (int i = 1; i < N; i++) {
        C[i] = A[i] + 10;
    }
}

