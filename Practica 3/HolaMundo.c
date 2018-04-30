#include <omp.h>
#include <stdio.h>

int main() {
  int nthreads, tid;

  #pragma omp parallel private(tid)
  {
    tid = omp_get_thread_num();
    printf("¡Hola todo el Mundo! Aquí el hilo %d.\n", tid);
    if(tid == 0){
      nthreads = omp_get_num_threads();
      printf("Número de hilos = %d.\n", nthreads);
    }
  }

  return 0;
}
