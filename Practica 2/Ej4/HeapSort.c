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

void PercolateUp (int * Heap, int tamanio, int valor) {
  int swap, posPadre, huboSwap = 1;
  while (tamanio > 0 && huboSwap == 1) {
    posPadre = tamanio * 0.5 - 1;
    swap = Heap[posPadre];
    if(valor < swap){
      Heap[posPadre] = valor;
      Heap[tamanio] = swap;
    } else {
      huboSwap = 0;
    }
    tamanio = posPadre;
  }
}
int HeapDelete(int * Heap, int tamanio) {
  int posMenor = 1, hijoMenor, posPadre = 0, huboSwap = 1;
  int retorno = Heap[0];
  int padre = Heap[tamanio];
  Heap[tamanio] = 0;
  while (posMenor < tamanio && huboSwap == 1) {
    //Elige el menor de los dos hijos del nodo
    hijoMenor = Heap[posMenor];
    if(posMenor + 1 < tamanio && Heap[posMenor + 1] < hijoMenor){
      posMenor = posMenor + 1;
      hijoMenor = Heap[posMenor];
    }
    //Compara al menor con el padre
    if(hijoMenor < padre){
      Heap[posPadre] = hijoMenor;
      Heap[posMenor] = padre;
      posPadre = posMenor;
      posMenor = posMenor * 2 + 1;
    } else {
      huboSwap = 0;
    }
  }
  return retorno;
}
void HeapSort(){
  int * Heap = (int*)malloc(sizeof(int)*N);
  int i, valor, tam = N - 1;
  for (i = 0; i < N; i++) {
    valor = A[i];
    Heap[i] = valor;
    PercolateUp(Heap, i, valor);
  }
  for (i = 0; i < N; i++) {
    int j;
    for (j = 0; j < tam; j++) {
      printf(" %d |", Heap[j]);
    }
    printf("\n");
    A[i] = HeapDelete(Heap, tam);
    tam--;
  }

}

int main(int argc, char * argv[]) {
  N = atoi(argv[1]);
  A=(int*)malloc(sizeof(int)*N);
  completaArreglo();

  HeapSort();

  chequeo();

  return 0;

}
