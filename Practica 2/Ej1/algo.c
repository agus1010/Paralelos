#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int N = 100, threadsc = 2;
double *A,*B,*C;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

void multiplicacion(int id){
  //hace la magia de la multiplicacion
  printf("MULTIPLICANDING");
  pthread_exit(NULL); //cuando finaliza el hilo (osea, dentro del hilo)
}

int main(int argc,char*argv[]){
 int i,j,k;
 int check=1;
 double timetick;

 //Controla los argumentos al programa
 if ((argc <= 2) || ((N = atoi(argv[1])) <= 0) )
  {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    exit(1);
  }

 //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);
  B=(double*)malloc(sizeof(double)*N*N);
  C=(double*)malloc(sizeof(double)*N*N);

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(i=0;i<N*N;i++){
    A[i] = 1;
    B[i] = 1;
    C[i] = 0;
  }

  //(CONSEJO) CREAR HILOS IGUAL AL NÚMERO DE PROCESADORES DISPONIBLES
  //DECLARA LOS THREADS
  if(threadsc = atoi(argv[2]) <= 1){
    threadsc = 2;
  }
  int ids[threadsc];
  pthread_t threads[threadsc];  //define los hilos
  double attr;
  pthread_attr_init(&attr);

  //CREA LOS HILOS
  timetick = dwalltime();
  for(i = 0; i<threads; i++){
    ids[i] = i;
    pthread_create(&threads[i], &attr, multiplicacion, &ids[i]);
  }
  pthread_join(threads,NULL); //esto, dentro de main

  printf("Tiempo en segundos %f\n", dwalltime() - timetick);
  //* = DIRECCIÓN DE MEMORIA (VALOR DEL PUNTERO)
  //& = ACCEDE AL DATO (EL DATO AL QUE APUNTA EL PUNTERO)

  //Realiza la multiplicacion

 //Verifica el resultado
  for(i=0;i<N*N;i++){
    check=check && C[i];
  }

  if(check){
   printf("Multiplicacion de matrices resultado correcto\n");
  }else{
   printf("Multiplicacion de matrices resultado erroneo\n");
  }

 free(A);
 free(B);
 free(C);
 return(0);
}
