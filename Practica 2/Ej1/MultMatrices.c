#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int N,
    cantThreads = 4,
    elemsPorThread,
    chunk;
int * A, * B, * C;

double dwalltime(){
  double sec;
  struct timeval tv;
  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
  return sec;
}
void inicializacion(int argc, char* argv[]){
  int i = 0;
  //Controla los parámetros
  if(argc != 2){
    N = atoi(argv[1]);
    if((cantThreads = atoi(argv[2])) < 0){
      cantThreads = 4;
    }
  } else {
    printf("Error de parámetros...\n");
    exit(0);
  }
  //Aloca memoria para las matrices
  A=(int*)malloc(sizeof(int)*N*N);
  B=(int*)malloc(sizeof(int)*N*N);
  C=(int*)malloc(sizeof(int)*N*N);
  //Completa las matrices
  for (i=0; i<N*N; i++){
    A[i] = 1;
    B[i] = 1;
    C[i] = 0;
  }
}
void controlaResultado(){
  //Verifica el resultado
  int i = 0,
      check = N;
  while(check == N && i < N*N){
    if((check = C[i])!=N){
      printf("Resultado Erróneo.\n");
    }
    i++;
  }
  if(check == N){
    printf("Resultado Correcto.\n");
  }
}

void * multiplica (void * ptr){
  int * p, id; p = (int *) ptr; id = *p;
  int i, j, k;
  int desp = id * elemsPorThread, final = (id + 1) * elemsPorThread;
  int res = 0;
  int a, b;
  for(i=desp;i<final;i++){
    for(j=0;j<N;j++){
      for(k=0;k<N;k++){
        res = res + A[i*N+k] * B[k+j*N];
      }
      C[i*N+j] = res;
      res = 0;
    }
  }
}

int main(int argc, char* argv[]){
  int i;
  inicializacion(argc, argv);

  elemsPorThread = N / cantThreads;
  chunk = elemsPorThread / N;

  int ids[cantThreads];
  pthread_t threads[cantThreads];
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  double timetick = dwalltime();

  for (i = 0; i < cantThreads; i++) {
    ids[i] = i;
    pthread_create(&threads[i], &attr, multiplica, &ids[i]);
  }
  // Espera a que los hilos terminen
  for (i = 0; i < cantThreads; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Tiempo en segundos %f\n", dwalltime() - timetick);

  controlaResultado();

  return 0;

}
