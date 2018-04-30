//Ejercicio 2
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main(int argc,char*argv[]){
  //INICIALIZACIÓN
  double x,scale;
  int i;
  int numThreads = atoi(argv[2]);
  int N=atoi(argv[1]);
  omp_set_num_threads(numThreads);

  scale=2.78;
  x=0.0;
  double resultado = 0.0;

  //El problema estaba en que todos los hilos generados pisaban el resultado
  //final con su propio resultado. Era un problema de exclusión mutua.
  #pragma omp parallel private(x)
  {
    #pragma omp for private(i)
    for(i=1;i<=N;i++){
      x= x + sqrt(i*scale) + 2*x;
    }
    #pragma omp critical
    resultado = resultado + x;
  }

  printf("\n Resultado: %f \n",resultado);

  return(0);
}
