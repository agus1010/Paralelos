#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int threadCount,
    threadElems,
    chunk,
    N;
double *A, *B, *C;

//Para calcular tiempo
double dwalltime() {
  double sec;
  struct timeval tv;
  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
  return sec;
}

void * multiplica (void * ptr);

int main(int argc,char*argv[]) {

  if(argc == 2){
    N = atoi(argv[1]);
    threadCount = atoi(argv[2]);
  } else {
    printf("Parámetros: Dimensión de la matriz (N) y cantidad de threads.\n", );
  }
  threadElems = N*N / threadCount;
  double timetick;
  int i, ids[threadCount];

  //threadCount = atoi(argv[2]);

  //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);
  B=(double*)malloc(sizeof(double)*N*N);
  C=(double*)malloc(sizeof(double)*N*N);

  pthread_t threads[threadCount];
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  chunk = N / threadCount;

  timetick = dwalltime();
  // Crea los hilos
  for (i = 0; i < threadCount; i++) {
    ids[i] = i;
    pthread_create(&threads[i], &attr, multiplica, &ids[i]);
  }
  // Espera a que los hilos terminen
  for (i = 0; i < threadCount; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Tiempo en segundos %f\n", dwalltime() - timetick);

  //Verifica el resultado
  i = 0;
  int check = 0;
  while (check == 0 && i < N*N){
    check = C[i];
    i++;
  }
  if(check == 0){
    printf("Resultado Correcto.\n");
  } else {
    printf("Resultado Erróneo.\n");
  }

  return 0;
}

void * multiplica (void * ptr){
  int * p, id; p = (int *) ptr; id = *p;
  int i, j;
  int desp = threadElems * id;
  int fin = (id + 1) * threadElems;
  for (i = desp; i < fin; i++) {
    for (j = desp; j < fin; j++) {
      C[i] = C[i] + A[i] * B[j];
    }
  }

}
