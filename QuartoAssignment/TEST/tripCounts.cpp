
#define N 15


//====================================================
// CASE 1
// 
// STESSO T.C.
//====================================================
void sameTC(int *A, int*B) {
    for(int i=0; i<N; i++) {
        A[i] +=1;
    }
    for(int i=0; i<N; i++) {
        B[i] -=1;
    }
}



//====================================================
// CASE 2
// VALORE PER RIFERIMENTO
// STESSO T.C.
//====================================================
void sameTC_ref(int *A, int*B, int z) {
    for(int i=0; i<z; i++) {
        A[i] +=1;
    }
    for(int i=0; i<z; i++) {
        B[i] -=1;
    }
}



//====================================================
// CASE 3
// VALORE PER RIFERIMENTO
// T.C. DIFFERENTE
//====================================================
void sameTC_ref_diff(int *A, int*B, int z) {

    int q = z-5;

    for(int i=0; i<z; i++) {
        A[i] +=1;
    }
    for(int i=0; i<q; i++) {
        B[i] -=1;
    }
}



//====================================================
// CASE 4
// VALORE STATICO
// T.C. UGUALE
//====================================================
void sameTC_static(int *A, int*B) {

    for(int i=0; i<5; i++) {
        A[i] +=1;
    }
    for(int i=0; i<5; i++) {
        B[i] -=1;
    }
}




//====================================================
// CASE 5
// VALORE STATICO
// T.C. DIFFERENTE
//====================================================
void sameTC_static_diff(int *A, int*B) {

    for(int i=0; i<5; i++) {
        A[i] +=1;
    }
    for(int i=0; i<1; i++) {
        B[i] -=1;
    }
}




//====================================================
// CASE 6
// VALORE PER RIFERIMENTO
// guardia di uguaglianza
// T.C. UGUALE
//====================================================
void sameTC_equal(int *A, int*B, int q, int z) {
    if(q==z) {
        
        for(int i=0; i<q; i++) {
            A[i] +=1;
        }
        for(int i=0; i<z; i++) {
            B[i] -=1;
        }

    }
}




//====================================================
// CASE 7
// VALORE PER RIFERIMENTO
// cambio di valore
// T.C. UGUALE
//====================================================
void sameTC_modified(int *A, int*B, int q, int z) {
    q = z;
        
    for(int i=0; i<q; i++) {
        A[i] +=1;
    }
    for(int i=0; i<z; i++) {
        B[i] -=1;
    }
}




//====================================================
// CASE 8
// VALORE PER RIFERIMENTO
// cambio di valore
// T.C. DIFFERENTE
//====================================================
void sameTC_assigned(int *A, int*B, int q, int z) {
    q = 15;
    z = 20;
        
    for(int i=0; i<q; i++) {
        A[i] +=1;
    }
    for(int i=0; i<z; i++) {
        B[i] -=1;
    }
}




//====================================================
// CASE 9
// VALORE PER RIFERIMENTO
// cambio di valore
// T.C. UGUALE
//====================================================
void sameTC_assignedEqual(int *A, int*B, int q, int z) {
    q = 15;
    z = 15;
        
    for(int i=0; i<q; i++) {
        A[i] +=1;
    }
    for(int i=0; i<z; i++) {
        B[i] -=1;
    }
}


//====================================================
// CASE 10
// VALORE PER RIFERIMENTO
// doppia guardia di uguaglianza
// T.C. UGUALE
//====================================================
void sameTC_equalIn2(int *A, int*B, int q, int z) {
    if(q<=z) {
        if(q>=z) {
   
            for(int i=0; i<q; i++) {
                A[i] +=1;
            }
            for(int i=0; i<z; i++) {
                B[i] -=1;
            }
            
        }
    }
}


//====================================================
// CASE 11
// VALORE PER RIFERIMENTO
// i != 0
// T.C. DIFFERENTE
//====================================================
void sameTC_differentStart(int *A, int*B, int q, int z) {
    q = 15;
    z = 15;
        
    for(int i=7; i<q; i++) {
        A[i] +=1;
    }
    for(int i=0; i<z; i++) {
        B[i] -=1;
    }
}



//====================================================
// CASE 12
// i aggiornato in maniera differente
// 
// T.C. DIFFERENTE
//====================================================
void sameTC_differentUpdate(int *A, int*B, int q, int z) {
    q = 15;
    z = 15;
        
    for(int i=0; i<q; i++) {
        A[i] +=1;
    }
    for(int i=0; i<z; i++) {
        i++;
        B[i] -=1;
    }
}


//====================================================
// CASE 13
// i inverso rispetto al primo
// 
// T.C. UGUALE
//====================================================
void sameTC_differentTripDirection(int *A, int*B, int q, int z) {
    q = 15;
    z = 15;
        
    for(int i=0; i<q; i++) {
        A[i] +=1;
    }
    for(int i=z; i>0; i--) {
        B[i] -=1;
    }
}


//====================================================
// CASE 14
// FOR E DO-WHILE   
//
// T.C. UGUALE
//====================================================
void sameTC_doWAndFor(int *A, int*B, int q, int z) {
    q = 15;
    z = 15;
        
    for(int i=0; i<q; i++) {
        A[i] +=1;
    }
    int j=0;
    if(z > 0) {
        do {
            B[j] -=1;
            j++;
        } while(j < z);
    }
}


//====================================================
// CASE 15
// infinito
// 
// T.C. UGUALE
//====================================================
void sameTC_infinite(int *A, int*B, int q, int z) {
    q = 15;
    z = 15;
        
    while(true) {
        A[0] +=1;
    }
    while(true) {
        B[0] -=1;
    }
}





int main() {

    int A[N];
    int B[N];
    int C[N];

    sameTC(A,B);
    sameTC_ref(A,B, 12);
    sameTC_ref_diff(A,B, 12);
    sameTC_static(A,B);
    sameTC_static_diff(A,B);
    sameTC_equal(A,B, 12, 15);
    sameTC_equal(A,B, 12, 12);
    sameTC_modified(A,B, 12, 15);
    sameTC_assigned(A,B, 12, 15);
    sameTC_assignedEqual(A,B, 10, 10);
    sameTC_equalIn2(A,B, 14, 16);


    return 0;
}