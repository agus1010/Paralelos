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

  for (i = 0; i < N*N; i++) {
    B[i] = 1;
  }

  //MPI_Scatter (void *sendbuf, int cantEnvio, MPI_Datatype tipoDatoEnvio, void*recvbuf, int cantRec, MPI_Datatype tipoDatoRec, int origen, MPI_Comm comunicador)
  if (id == 0) {
    A=(double*)malloc(sizeof(double)*N*N);
    C=(double*)malloc(sizeof(double)*N*N);
    for (size_t i = 0; i < N*N; i++) {
      A[i] = 1;
      C[i] = 0;
    }
    MPI_Bcast(B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double timetick = dwalltime();
    MPI_Scatter(A,elemsPorProc,MPI_DOUBLE,A,elemsPorProc,MPI_DOUBLE,0,MPI_COMM_WORLD);
    for (i = 0; i < elemsPorProc; i++) {
      for (j = 0; j < N; j++) {
        for (k = 0; k < N; k++) {
          C[i] += A[i*N+k] * B[k+j*N];
        }
      }
    }
    //MPI_Gather (void *sendbuf, int cantEnvio, MPI_Datatype tipoDatoEnvio, void*recvbuf, int cantRec, MPI_Datatype tipoDatoRec, int destino, MPI_Comm comunicador)
    MPI_Gather(strip, elemsPorProc, MPI_DOUBLE, A, elemsPorProc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    printf("El tiempo en segundos es %f.\n", dwalltime() - timetick);
    /*printf("A:");
    for (i = 0; i < N; i++) {
      printf("\n|");
      for (j = 0; j < N; j++) {
        printf(" %f |", C[i]);
      }
    }
    printf("\n");*/
  } else {
    strip=(double*)malloc(sizeof(double)*elemsPorProc);
    A=(double*)malloc(sizeof(double)*elemsPorProc);
    MPI_Bcast(B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(A,elemsPorProc,MPI_DOUBLE,A,elemsPorProc,MPI_DOUBLE,0,MPI_COMM_WORLD);
    for (i = 0; i < elemsPorProc; i++) {
      for (j = 0; j < N; j++) {
        for (k = 0; k < N; k++) {
          strip[i] += A[i*N+k] * B[k+j*N];
        }
      }
    }
    MPI_Gather(strip, elemsPorProc, MPI_DOUBLE, A, elemsPorProc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
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
