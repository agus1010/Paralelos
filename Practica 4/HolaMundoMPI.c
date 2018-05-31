#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int cantidad, identificador;

  /* iniciar entorno MPI */
  MPI_Init(&argc, &argv);
  /* obtener cantidad de procesos */
  MPI_Comm_size(MPI_COMM_WORLD, &cantidad);
  /* obtener rank */
  MPI_Comm_rank(MPI_COMM_WORLD, &identificador);
  /* imprimir */
  printf("!Hola Mundo! Soy %d de %d \n", identificador + 1, cantidad);
  /* finalizar entorno MPI */
  MPI_Finalize();
}
