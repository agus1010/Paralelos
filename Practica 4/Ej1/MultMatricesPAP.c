#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double dwalltime();

int main(int argc, char * argv[]) {

  int N = atoi(argv[1]);
  int cant, id;
  MPI_Status estado;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &cant);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);


  int i, j, k;
  int elemsPorProc = (N*N)/cant;

  double * A, * B, * C, * strip;
  B=(double*)malloc(sizeof(double)*N*N);

  if(id == 0)
  {
    A=(double*)malloc(sizeof(double)*N*N);
    C=(double*)malloc(sizeof(double)*N*N);
    for (i = 0; i < N*N; i++) {
      A[i] = 1;
      B[i] = 1;
      C[i] = 0;
    }
    MPI_Bcast(B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double timetick = dwalltime();
    for (i = 1; i < cant; i++) {
      MPI_Send(A+i*elemsPorProc, elemsPorProc, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
    }
    for (i = 0; i < elemsPorProc; i++) {
      for (j = 0; j < N; j++) {
        for (k = 0; k < N; k++) {
          C[i] += A[i*N+k] * B[k+j*N];
        }
      }
    }
    for (i = 1; i < cant; i++) {
      MPI_Recv(C+i*elemsPorProc, elemsPorProc, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
      /*
      estado.source.
      estado.tag.
      */
    }
    printf("El tiempo en segundos es %f.\n", dwalltime() - timetick);
  }
  else
  {
    A=(double*)malloc(sizeof(double)*elemsPorProc);
    strip=(double*)malloc(sizeof(double)*elemsPorProc);
    for (i = 0; i < elemsPorProc; i++) {
      A[i] = 0;
      strip[i] = 0;
    }
    MPI_Bcast(B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Recv(A, elemsPorProc, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &estado);
    for (i = 0; i < elemsPorProc; i++) {
      for (j = 0; j < N; j++) {
        for (k = 0; k < N; k++) {
          strip[i] += A[i*N+k] * B[k+j*N];
        }
      }
    }
    MPI_Send(strip, elemsPorProc, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return 0;
}

/* ------------------ ADICIONALES ------------------ */
#include <sys/time.h>
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}
