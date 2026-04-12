#include <stdio.h>

// ==========================
// 1. CASO BASE (Standard)
// ==========================
// Il pattern esatto descritto nell'assignment: (b + 1) - 1
int test_basic(int b) {
    int a = b + 1;
    int c = a - 1;
    return c; // Dovrebbe diventare semplicemente 'return b'
}

// ==========================
// 2. CASO COMMUTATIVO
// ==========================
// L'addizione è commutativa, il pass dovrebbe gestire anche (1 + b) - 1
int test_commutative(int b) {
    int a = 1 + b;
    int c = a - 1;
    return c; // Dovrebbe diventare semplicemente 'return b'
}

// ==========================
// 3. CASO CON INTERFERENZE
// ==========================
// LLVM guarda le dipendenze dei dati (grafo Def-Use), non l'ordine 
// testuale. Questa funzione testa se il pass trova la dipendenza 
// anche se ci sono altre istruzioni in mezzo.
int test_interleaved(int b, int x) {
    int a = b + 1;
    int y = x * 2; // Istruzione scollegata, non deve dare fastidio
    int c = a - 1;
    return c + y;  // Dovrebbe diventare 'return b + y'
}

// ==========================
// 4. CASO NEGATIVO in cui non viene modificato niente
// ==========================
// Qui i numeri non combaciano (si somma 2 e si sottrae 1).
// Il pass NON deve ottimizzare questa funzione!
int test_negative(int b) {
    int a = b + 2;
    int c = a - 1;
    return c; 
}

