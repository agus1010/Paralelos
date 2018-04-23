#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int N, cantThreads, elemsPorThread;
int * A, * Resultado;

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
  Resultado = (int*)malloc(sizeof(int)*cantThreads);
  //Completa el arreglo
  completaArreglo();
}
double dwalltime(){
  double sec;
  struct timeval tv;
  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
  return sec;
}
void chequeo(double promedio){
  int i, total = 0;
  double resultado;
  for (i = 0; i < N; i++) {
    total = total + A[i];
  }
  resultado = total / N;
  if(resultado == promedio){
    printf("Resultado Correcto,\n");
  } else {
    printf("Resultado Erróneo. Promedio obtenido: %f, Promedio chequeo: %f.\n", promedio, resultado);
  }
}

void * suma (void * ptr){
  int * p, id, resultado; p = (int *) ptr; id = *p;  //Calcula el ID del thread
  int i, total = 0;
  for (i = id * elemsPorThread; i < (id + 1) * elemsPorThread; i++) {
    total = total + A[i];
  }
  Resultado[id] = total;
}

int main(int argc, char * argv[]) {

  inicializacion(argc, argv);

  elemsPorThread = N / cantThreads;
  int ids[cantThreads], i, total = 0;
  double resultado;
  pthread_t threads[cantThreads];
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  double timetick = dwalltime();

  for (i = 0; i < cantThreads; i++) {
    ids[i] = i;
    pthread_create(&threads[i], &attr, suma, &ids[i]);
  }
  // Espera a que los hilos terminen
  for (i = 0; i < cantThreads; i++) {
    pthread_join(threads[i], NULL);
  }
  for (i = 0; i < cantThreads; i++){
    total = total + Resultado[i];
  }
  resultado = total / N;

  printf("Tiempo en segundos %f.\n", dwalltime() - timetick);
  printf("El valor promedio del arreglo es: %f.\n", resultado);

  chequeo(resultado);

  return 0;
}
