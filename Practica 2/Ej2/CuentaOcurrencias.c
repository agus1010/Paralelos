#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int N,
    cantThreads = 4,
    elemsPorThread,
    numBuscado;
int * A, * Resultado;

void completaArreglo(){
  int i, j;
  // Inicializa el generador de números random
  time_t t;
  srand((unsigned) time(&t));
  //Completa el arreglo con un número de apariciones random del número buscado
  for (i=0; i<N; i++){
    j = rand() % 10;
    if(j == 5){
      A[i] = numBuscado;
    }
  }
}
void inicializacion(int argc, char* argv[]){
  int i = 0;
  //Controla los parámetros
  if(argc != 3){
    N = atoi(argv[1]);
    numBuscado = atoi(argv[2]);
    if((cantThreads = atoi(argv[3])) < 0){
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

/* Proceso de los trheads */
void * cuentaOcurrencias (void * ptr){
  int * p, id, resultado; p = (int *) ptr; id = *p;  //Calcula el ID del thread
  int i,
      desp = id * elemsPorThread,
      final = desp + elemsPorThread,
      cantOcurrencias = 0,
      actual;
  for (i = desp; i < final; i++) {
    if((actual = A[i])==numBuscado){
      cantOcurrencias++;
    }
  }
  Resultado[id] = cantOcurrencias;
}

int main(int argc, char * argv[]) {

  inicializacion(argc, argv);

  elemsPorThread = N / cantThreads;
  int ids[cantThreads], i, salida = 0, ocurrencias = 0;
  pthread_t threads[cantThreads];
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  double timetick = dwalltime();

  for (i = 0; i < cantThreads; i++) {
    ids[i] = i;
    pthread_create(&threads[i], &attr, cuentaOcurrencias, &ids[i]);
  }
  // Espera a que los hilos terminen
  for (i = 0; i < cantThreads; i++) {
    pthread_join(threads[i], NULL);
  }
  for (i = 0; i < cantThreads; i++) {
    ocurrencias = ocurrencias + Resultado[i];
  }

  printf("Tiempo en segundos %f\n", dwalltime() - timetick);

  printf("El numero %d aparece %d veces en un arreglo de longitud %d.\n", numBuscado, ocurrencias, N);

  //Controla el resultado
  int check = 0;
  for (i = 0; i < N; i++) {
    if(A[i]==numBuscado){
      check++;
    }
  }
  if(ocurrencias == check){
    printf("Resultado correcto.\n");
  } else {
    printf("Resultado incorrecto.\n");
  }
  
  return 0;
}
