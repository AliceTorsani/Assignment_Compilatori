#include <iostream>
#include <cmath>
// test con loop annidati
int main(){
  const int n=200; //Dimensione dei loop
  double a=10.0;
  double b=20.0;
  for (int i=0; i<n;++i){
    // istruzione loop invariant
    double c=a+b;
    for (int j=0;j<n;++j){
      // loop-invariant per il ciclo j 
      double radiceA=std::sqrt(a+i);
      // loop invariant
      double doppioA=a*2;
      for (int k=0;k<n;++k){
        // loop invariant per il ciclo k 
        double radiceB=std::sqrt(b+j);
        double radiceC=std::sqrt(c+k);
        double radici=radiceA+radiceB+radiceC;
      }
    }
  }
}
