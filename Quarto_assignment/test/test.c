// test.c

// CASO : POSITIVO
// I loop sono adiacenti e control flow equivalent.
// L0 domina L1 e L1 postdomina L0. Nessun blocco extra tra uscita L0 ed entry L1.
void test_fusion_success(int *A, int *B, int *C, int N) {
    // Loop 0
    for (int i = 0; i < N; i++) {
        A[i] = B[i] * 2;
    }

    // Loop 1
    for (int i = 0; i < N; i++) {
        C[i] = A[i] + 10;
    }
}

// CASO : NEGATIVO (Fallisce l'adiacenza)
// I loop NON sono adiacenti.
void test_fusion_fail_adjacency(int *A, int *B, int *C, int N) {
    // Loop 0
    for (int i = 0; i < N; i++) {
        A[i] = B[i] * 2;
    }

    // Questa singola istruzione genera un Basic Block intermedio
    // tra l'uscita del primo loop e il preheader del secondo.
    A[0] = 0; 

    // Loop 1
    for (int i = 0; i < N; i++) {
        C[i] = A[i] + 10;
    }
}

// CASO : NEGATIVO (Fallisce l'equivalenza del flusso di controllo)
// I loop NON sono control flow equivalenti.
void test_fusion_fail_cf_equivalence(int *A, int *B, int *C, int N, int cond) {
    // Loop 0
    for (int i = 0; i < N; i++) {
        A[i] = B[i] * 2;
    }

    // Questa condizione rompe l'equivalenza del flusso di controllo.
    // L1 non postdomina L0 perché il flusso potrebbe saltare L1.
    if (cond) {
        // Loop 1
        for (int i = 0; i < N; i++) {
            C[i] = A[i] + 10;
        }
    }
}
