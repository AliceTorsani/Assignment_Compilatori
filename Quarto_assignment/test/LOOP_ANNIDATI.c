// =====================================================================
// TEST: LOOP ANNIDATI
// =====================================================================

// CASO 1: POSITIVO 
// Ci sono due loop interni adiacenti allo stesso livello di profondità (depth = 2)
// all'interno di un loop esterno (depth = 1). Il pass dovrebbe fondere i due loop interni.
void test_nested_fusion_success(int *A, int *B, int *C, int N, int M) {
    // Loop Esterno (Depth 1)
    for (int i = 0; i < N; i++) {
        
        // Inner Loop 0 (Depth 2)
        for (int j = 0; j < M; j++) {
            A[i * M + j] = B[i * M + j] * 2;
        }

        // Inner Loop 1 (Depth 2) - Adiacente al precedente
        for (int j = 0; j < M; j++) {
            C[i * M + j] = A[i * M + j] + 10;
        }
    }
}

// CASO 2: NEGATIVO (Fallisce l'adiacenza tra Inner Loops)
// C'è un'istruzione in mezzo ai due loop interni che appartiene al loop esterno.
// Questo rompe l'adiacenza sicura.
void test_nested_fusion_fail_adjacency(int *A, int *B, int *C, int N, int M) {
    // Loop Esterno (Depth 1)
    for (int i = 0; i < N; i++) {
        
        // Inner Loop 0 (Depth 2)
        for (int j = 0; j < M; j++) {
            A[i * M + j] = B[i * M + j] * 2;
        }

        // Istruzione intrusiva nel loop esterno! 
        // Modifica la memoria, quindi isIntermediateBlockSafe restituirà false.
        A[i * M] = 0; 

        // Inner Loop 1 (Depth 2)
        for (int j = 0; j < M; j++) {
            C[i * M + j] = A[i * M + j] + 10;
        }
    }
}
