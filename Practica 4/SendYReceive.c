#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double dwalltime();

// MPI_Send(void * buff, int cant, MPI_Datatype tipoDato, int destino, int tag, MPI_Comm comunicador);
// MPI_Recv(void * buff, int cant, MPI_Datatype tipoDato, int origen, int tag, MPI_Comm comunicador, MPI_Status * estado);

int main(int argc, char *argv[]) {

  MPI_Init(&argc,&argv);

  int cant, id;
  int a[10], b[10];
  MPI_Status estado;
  MPI_Comm_size(MPI_COMM_WORLD, &cant);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  int i;

  if(id == 0) {
    for (i = 0; i < 10; i++) {
      a[i] = i;
      b[i] = 10 - i;
    }
    printf("A:\n|");
    for (i = 0; i < 10; i++) {
      printf(" %d |", a[i]);
    }
    printf("\nB:\n|");
    for (i = 0; i < 10; i++) {
      printf(" %d |", b[i]);
    }
    printf("\n");
    MPI_Send(a, 10, MPI_INT, 1, 1, MPI_COMM_WORLD);
    MPI_Send(b, 10, MPI_INT, 1, 2, MPI_COMM_WORLD);
    MPI_Recv(b, 10, MPI_INT, 1, 1, MPI_COMM_WORLD, &estado);
    MPI_Recv(a, 10, MPI_INT, 1, 2, MPI_COMM_WORLD, &estado);
    printf("¡Y AHORA, OTRO SWAP DISTRIBUIDO USANDO SEND Y RECEIVE!\nA:\n|");
    for (i = 0; i < 10; i++) {
      printf(" %d |", a[i]);
    }
    printf("\nB:\n|");
    for (i = 0; i < 10; i++) {
      printf(" %d |", b[i]);
    }
    printf("\n");
  } else {
    MPI_Recv(b, 10, MPI_INT, 0, 1, MPI_COMM_WORLD, &estado);
    MPI_Recv(a, 10, MPI_INT, 0, 2, MPI_COMM_WORLD, &estado);
    printf("¡Y AHORA, UN SWAP DISTRIBUIDO USANDO SEND Y RECEIVE!\nA:\n|");
    for (i = 0; i < 10; i++) {
      printf(" %d |", a[i]);
    }
    printf("\nB:\n|");
    for (i = 0; i < 10; i++) {
      printf(" %d |", b[i]);
    }
    printf("\n");
    MPI_Send(a, 10, MPI_INT, 0, 1, MPI_COMM_WORLD);
    MPI_Send(b, 10, MPI_INT, 0, 2, MPI_COMM_WORLD);
  }
  MPI_Finalize();

  return 0;
}
































/*#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double dwalltime();

int main(int argc, char * argv[]) {

  int id, cantProcesos;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &cantProcesos);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  int N = atoi(argv[1]);

  int elemsPorProceso = (N*N)/cantProcesos;

  double * B = (double*)malloc(sizeof(double)*N*N);
  double * strip = (double*)malloc(sizeof(double)*elemsPorProceso);
  MPI_Status estado;
  int i, j, k, res;
  int desp;

  //punto a punto
  if(id == 0) {
    double * A = (double*)malloc(sizeof(double)*N*N);
    double * C = (double*)malloc(sizeof(double)*N*N);
    for (i = 0; i < N*N; i++) {
      A[i] = 1;
      B[i] = 1;
      C[i] = 0;
    }
    MPI_Bcast(B,N*N,MPI_DOUBLE,0,MPI_COMM_WORLD);
    //Envía porción de matriz a todos los procesos;
    for (i = 1; i < cantProcesos; i++) {
      desp += elemsPorProceso;
      MPI_Send(A+desp,elemsPorProceso,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
    }

    // EMPIEZA CÓMPUTO
    double timetick = dwalltime();

    for (i = desp; i < N; i++) {
      for (j = 0; j < N; j++) {
        res = 0;
        for (k = 0; k < N; k++) {
          res += A[i*N+k] * B[i+k*N];
        }
        C[i*N+j] = res;
      }
    }
    desp = 0;
    for (i = 1; i < cantProcesos; i++) {
      MPI_Recv(C+((i-1)*desp),elemsPorProceso,MPI_DOUBLE,i,MPI_ANY_TAG,MPI_COMM_WORLD,&estado);
      desp += elemsPorProceso;
    }

    printf("Tiempo en segundos %f\n", dwalltime() - timetick);

  } else {
    double * stripRes = (double*)malloc(sizeof(double)*elemsPorProceso);
    MPI_Bcast(B,N*N,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Recv(strip,elemsPorProceso,MPI_DOUBLE,0,MPI_ANY_TAG,MPI_COMM_WORLD,&estado);
    desp = (id-1) * elemsPorProceso;
    for (i = desp; i < N; i++) {
      for (j = 0; j < N; j++) {
        res = 0;
        for (k = 0; k < N; k++) {
          res += strip[i*N+k] * B[i+k*N];
        }
        stripRes[i*N+j] = res;
      }
    }
    MPI_Send(stripRes,elemsPorProceso,MPI_DOUBLE,0,id,MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return 0;
}
*/
/* ------------------ ADICIONALES ------------------ */
#include <sys/time.h>
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}
