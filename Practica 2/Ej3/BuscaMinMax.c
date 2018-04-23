#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

int * A, MAXIMO = -1, MINIMO = INT_MAX;
int N, cantThreads, elemsPorThread;
pthread_mutex_t minimum_value_lock;

void completaArreglo(){
  int i, j;
  // Inicializa el generador de números random
  time_t t;
  srand((unsigned) time(&t));
  //Completa el arreglo con un número de apariciones random del número buscado
  for (i=0; i<N; i++){
    j = rand() % 10;
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
void chequeo(){
  int min = INT_MAX, max = -1, i = 0;
  for(i = 0; i<N; i++) {
    if(A[i] < min){
      min = A[i];
    }
    if(A[i] > max){
      max = A[i];
    }
    i++;
  }
  if(min == MINIMO && max == MAXIMO){
    printf("Resultado Correcto.\n");
  } else {
    printf("Resultado Erróneo. MIN = %d, MAX = %d.\n", min, max);
  }
}
double dwalltime(){
  double sec;
  struct timeval tv;
  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
  return sec;
}

void * busqueda(void * ptr){
  int * p, id, resultado; p = (int *) ptr; id = *p;  //Calcula el ID del thread
  int i, actual, ultMIN = INT_MAX, ultMAX = -1, cambiaMIN = 0, cambiaMAX = 0;
  for (i = id * elemsPorThread; i < (id + 1) * elemsPorThread; i++) {
    actual = A[i];
    if(actual < ultMIN){
      ultMIN = actual;
      cambiaMIN = 1;
    }
    if(actual > ultMAX){
      ultMAX = actual;
      cambiaMAX = 1;
    }
    if(cambiaMIN == 1 || cambiaMAX == 1){
      pthread_mutex_lock(&minimum_value_lock);
      if(actual < MINIMO){
        MINIMO = actual;
      }
      if(actual > MAXIMO){
        MAXIMO = actual;
      }
      pthread_mutex_unlock(&minimum_value_lock);
      cambiaMIN = 0;
      cambiaMAX = 0;
    }
  }
}

int main(int argc, char * argv[]) {

  inicializacion(argc, argv);

  elemsPorThread = N / cantThreads;
  int ids[cantThreads], i;
  pthread_t threads[cantThreads];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_mutex_init (&minimum_value_lock, NULL);

  double timetick = dwalltime();

  for (i = 0; i < cantThreads; i++) {
    ids[i] = i;
    pthread_create(&threads[i], &attr, busqueda, &ids[i]);
  }
  // Espera a que los hilos terminen
  for (i = 0; i < cantThreads; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Tiempo en segundos %f.\n", dwalltime() - timetick);
  printf("El mínimo es: %d, y el máximo es: %d.\n", MINIMO, MAXIMO);

  chequeo();

  return 0;
}
