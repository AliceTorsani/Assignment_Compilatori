void test1_to_3(int a) {
    int n = 5;
    int i=0;
    do
    {
        i++;
        a--;
    } while (n>=i);

    for (int i = 0; i < n; i++)
    {
        a += i;
    }

    //stesso trip count

    for (int q = 0; q < n; q++) {
        a -= 2*q;
    }

    //stesso trip count
    
    for (int z = 0; z<n; z++) {
        z += 2;
    }

    //trip count diverso
}
//VVF

void test4_to_7(int r) {
    int q=7;
    int j = q;
    for(int i=0; i<q; i++) {
        j += i * q;
    }
    int i = 0;
    while(i < q) {
        j *= i;
        i++;
    }
    //stesso trip count
    i=0;
    do {
        i++;
    }while(i<q);
    //trip count differente
    int q1 = q;
    for(int i=0; i+1<q; i++) {
        q1--;
    }
    //trip count uguale
    for(int i=0; i+1<q1; i++) {
        q1--;
    }
    //trip count differente
}
//VFVF