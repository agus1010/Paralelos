#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

/* Time in seconds from some point in the past */
double dwalltime();

int N;
double *A,*B,*C;

void printResultado(){
  int i,c;
  for (i = 0; i < N*N; i++) {
    c = C[i];
    printf(" %d |", c);
  }
  printf("\n");
}

int main(int argc,char*argv[]){
 int i,j,k;
 int check=1;
 double timetick;

 //Controla los argumentos al programa
  if (argc < 3){
   printf("\n Faltan argumentos:: N dimension de la matriz, T cantidad de threads \n");
   return 0;
  }
  N=atoi(argv[1]);
  int numThreads=atoi(argv[2]);
  omp_set_num_threads(numThreads);


 //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);
  B=(double*)malloc(sizeof(double)*N*N);
  C=(double*)malloc(sizeof(double)*N*N);

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
     A[i*N+j]=1;
     C[i*N+j]=0;
     B[i+j*N]=1;
   }
  }

  int desp, final;
  int elemsPorThread = N / numThreads;
  int resultado;

  timetick = dwalltime();
  //Realiza la multiplicacion
  #pragma omp parallel private(desp,final,i)
  {
    desp = omp_get_thread_num() * elemsPorThread;
    final = (omp_get_thread_num()+1) * elemsPorThread;
    for(i=desp;i<final;i++){
      #pragma omp parallel for private(j,k,resultado)
      for(j=omp_get_thread_num()*N;j<(omp_get_thread_num()+1)*N;j++){
        resultado = 0;
        for(k=0;k<N;k++){
          resultado = resultado + A[i*N+k]*B[k+j*N];
        }
        C[i*N+j] = resultado;
      }
    }
  }
  printf("Tiempo en segundos %f \n", dwalltime() - timetick);


 //Verifica el resultado
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	check=check&&(C[i*N+j]==N);
   }
  }

  if(check){
   printf("Multiplicacion de matrices resultado correcto\n");
  }else{
   printf("Multiplicacion de matrices resultado erroneo\n");
  }

  //printResultado();

 free(A);
 free(B);
 free(C);
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