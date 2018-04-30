#include <stdio.h>
#include <stdlib.h>

int * A;
int N;

void completaArreglo(){
  int i, j;
  // Inicializa el generador de números random
  time_t t;
  srand((unsigned) time(&t));
  //Completa el arreglo con un número de apariciones random del número buscado
  for (i=0; i<N; i++){
    j = rand() % 501;
    A[i] = j;
  }
}
void chequeo(){
  int i = 1;
  while (i < N && A[i-1] <= A[i]) {
    i++;
  }
  if(i == N){
    printf("Resultado Correcto.\n");
  } else {
    printf("Resultado Erróneo. Posición: %d.\n", i);
  }
  int j;
  for (j = 0; j < N; j++) {
    if(j != i){
      printf("%d | ",A[j]);
    } else {
      printf("¿>%d<? | ",A[j]);
    }
  }
  printf("\n");
}

void ordenamiento() {
   int i, j, actual, siguiente;
   for (i = 0; i < N-1; i++){
     for (j = 0; j < N-i-1; j++){
       actual = A[j];
       siguiente = A[j+1];
       if (actual > siguiente){
         A[j] = siguiente;
         A[j+1] = actual;
       }
     }
   }
}

int main(int argc, char *argv[]) {

  N = atoi(argv[1]);
  A=(int*)malloc(sizeof(int)*N);
  completaArreglo();

  ordenamiento();

  chequeo();

  return 0;
}
