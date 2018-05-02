//traspuesta.c
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

/* Time in seconds from some point in the past */
double dwalltime();

void printEstado(int hilo, int pos){
  #pragma omp critical
    printf("Hilo: %d, Pos: %d.\n", hilo, pos);
}

int main(int argc,char*argv[]){
 double *A;
 double temp;
 int i,j, N,tid;
 int check=1;
 double timetick;

 //Controla los argumentos al programa
  if (argc < 3){
   printf("\n Faltan argumentos:: N dimension de la matriz, T cantidad de threads \n");
   return 0;
  }
   N=atoi(argv[1]);
   int numThreads = atoi(argv[2]);
   omp_set_num_threads(numThreads);

 //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);

  //Inicializa la matriz con unos en el triangulo inferior y ceros en el triangulo superior.
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
		if (i>=j)
		{
			A[i*N+j]= 1.0;
		}
		else
		{
			A[i*N+j]= 0.0;
		}

   }
  }

  int cantUnos = (N*(N+1))*0.5;
  int elemsPorThread = cantUnos/numThreads;
  int desp, final;

  #pragma omp parallel default(none) private(i,j,temp,timetick,tid,desp,final) shared(A,N,elemsPorThread)
  {
    tid = omp_get_thread_num();
    timetick = dwalltime();
    desp = tid * elemsPorThread;
    final = (tid + 1) * elemsPorThread;
    //Agregué schedule(dynamic,1), lo que distribuye mejor las iteraciones entre los hilos.
    //Asigna las iteraciones de manera no determinística sin dar prioridad a ningún hilo
    #pragma omp for schedule(dynamic,1) private(i,j,temp,desp,final) nowait
    for(i=0; i<N; i++){
      for(j=i+1;j<N;j++){
        temp = A[i*N+j];
        A[i*N+j]= A[j*N+i];
        A[j*N+i]= temp;
        printEstado(tid,i*N+j);
      }
    }
    printf("Tiempo en segundos para el thread %d: %f \n", tid,dwalltime() - timetick);
  }

  //Chequea los resultados
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
		if (i>j)
		{
			check= check&&(A[i*N+j]==0.0);
		}
		else
		{
			check= check&&(A[i*N+j]== 1.0);
		}

   }
  }

  if(check){
   printf("Resultado correcto\n");
  }else{
   printf("Resultado erroneo\n");
  }
 free(A);
 return(0);
}



/*****************************************************************/

#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}
