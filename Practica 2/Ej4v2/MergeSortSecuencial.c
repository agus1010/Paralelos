#include<stdlib.h>
#include<stdio.h>

int * A;
int N, cantThreads;

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
void inicializacion(int argc, char* argv[]){
  int i = 0;
  //Controla los parámetros
  if(argc != 2){
    N = atoi(argv[1]);
    if((cantThreads = atoi(argv[2])) < 1){
      cantThreads = 4;
    }
  } else {
    printf("Error de parámetros...\n");
    exit(0);
  }
  //Aloca el arreglo
  A=(int*)malloc(sizeof(int)*N);

  completaArreglo();
}
double dwalltime(){
  double sec;
  struct timeval tv;
  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
  return sec;
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
void imprimeArreglo() {
    int i;
    for (i=0; i < N; i++)
      printf("%d ", A[i]);
    printf("\n");
}

void merge(int l, int m, int r) {
  int i, j, k;
  int n1 = m - l + 1;
  int n2 =  r - m;
  /* create temp arrays */
  int L[n1], R[n2];
  /* Copy data to temp arrays L[] and R[] */
  for (i = 0; i < n1; i++){
    L[i] = A[l + i];
  }
  for (j = 0; j < n2; j++) {
    R[j] = A[m + 1+ j];
  }
  /* Merge the temp arrays back into A[l..r]*/
  i = 0; // Initial index of first subarray
  j = 0; // Initial index of second subarray
  k = l; // Initial index of merged subarray
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      A[k] = L[i];
      i++;
    } else {
      A[k] = R[j];
      j++;
    }
    k++;
  }
  /* Copy the remaining elements of L[], if there
  are any */
  while (i < n1) {
    A[k] = L[i];
    i++;
    k++;
  }
  /* Copy the remaining elements of R[], if there
  are any */
  while (j < n2) {
    A[k] = R[j];
    j++;
    k++;
  }
}

/* l is for left index and r is right index of the
   sub-array of A to be sorted */
void mergeSort(int l, int r) {
  if (l < r) {
    // Same as (l+r)/2, but avoids overflow for
    // large l and h
    int m = l+(r-l)/2;
    // Sort first and second halves
    mergeSort(l, m);
    mergeSort(m+1, r);
    merge(l, m, r);
  }
}

int main(int argc, char * argv[]) {
  inicializacion(argc, argv);

  double timetick = dwalltime();
  mergeSort(0, N-1);
  printf("Tiempo en segundos %f.\n", dwalltime() - timetick);

  imprimeArreglo();

  return 0;
}
