#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TRUE 1
#define FALSE 0

struct tarea {
  int r1;
  int r2;
};

int comprobar (int fila, int reinas[], int n);
int colocarReina (int fila, int reinas[], int n);

int main(int argc, char * argv[]) {

  int N = atoi(argv[1]);
  int cantWorkers, id;
  MPI_Status estado;
  MPI_Request request;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &cantWorkers);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  int * reinas = (int*)malloc(sizeof(int)*N);

  if (id == 0) {

    // GENERA TODAS LAS TAREAS
    int cantTareas = N*N;
    struct tarea * tareas = (struct tarea*)malloc(sizeof(struct tarea)*cantTareas);
    int i,j,pos;
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
        pos = i*N+j;
        tareas[pos].r1 = i;
        tareas[pos].r2 = j;
      }
    }

    //Envía a todos los workers la primer tarea y comienza a trabajar
    int * TareaEnvio = (int*)malloc(sizeof(int)*2);
    for (i = 1; i < cantWorkers; i++) {
      TareaEnvio[0] = tareas[i].r1;
      TareaEnvio[1] = tareas[i].r2;
      MPI_Send(TareaEnvio,2,MPI_INT,i,0,MPI_COMM_WORLD);
      cantTareas--;
    }
    // ENVÍA UNA TAREA A TODOS LOS WORKERS
    // MPI_Send(void * buff, int cant, MPI_Datatype tipoDato, int destino, int tag, MPI_Comm comunicador);
    // MPI_Recv(void * buff, int cant, MPI_Datatype tipoDato, int origen, int tag, MPI_Comm comunicador, MPI_Status * estado);
    //en estado.tag, está el id del emisor

    int mensajesRecibidos;
    int solucionesRecv;
    int total = 0;
    int flag = 0;

    while(cantTareas > 0) {
      //TRABAJAR
      reinas[0] = tareas[i].r1;
      reinas[1] = tareas[i].r2;
      i++;
      cantTareas--;
      if(comprobar(1,reinas,N)){
        total += colocarReina(2,reinas,N);
      }
      //Termina de trabajar y recibe resultados
      MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &estado);
      MPI_Get_count(&estado, MPI_INT, &mensajesRecibidos);
      while(mensajesRecibidos > 0) {  // MIENTRAS Haya RECVs pendientes: leerlos y luego dar trabajo
        //printf("HABÍA %d MENSAJES!\n", mensajesRecibidos);
        MPI_Recv(&solucionesRecv, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
        total += solucionesRecv;
        //printf("Mensaje enviado por: %d\n", estado.MPI_SOURCE);
        if(cantTareas > 0){
          TareaEnvio[0] = tareas[i].r1;
          TareaEnvio[1] = tareas[i].r2;
          MPI_Send(TareaEnvio,2,MPI_INT,estado.MPI_SOURCE,0,MPI_COMM_WORLD);
          cantTareas--;
          i++;
        }
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &estado);
        MPI_Get_count(&estado, MPI_INT, &mensajesRecibidos);
      }
    }

    //Protocolo de finalización CORREGIR - PUEDE HABER DEADLOCK
    for (i = 1; i < cantWorkers; i++) {
      MPI_Recv(&solucionesRecv, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
      total += solucionesRecv;
      TareaEnvio[0] = -1;
      MPI_Send(TareaEnvio, 2, MPI_INT, estado.MPI_SOURCE, 0, MPI_COMM_WORLD);
    }

    printf("Total de soluciones: %d\n", total);

  } else {

    int fin = 0;
    int * TareaRecv = (int*)malloc(sizeof(int)*2);
    int cantSoluciones = 0;
    while(fin == 0) {
      MPI_Recv(TareaRecv,2,MPI_INT,0,0,MPI_COMM_WORLD,&estado);
      if(TareaRecv[0] > -1) {
        reinas[0]=TareaRecv[0];
        reinas[1]=TareaRecv[1];
        if(comprobar(1,reinas,N)){
          cantSoluciones = colocarReina(2,reinas,N);
        } else {
          cantSoluciones = 0;
        }
        MPI_Send(&cantSoluciones,1,MPI_INT,0,id,MPI_COMM_WORLD);
      } else {
        fin = 1;
      }
    }

    printf("El hilo %d TERMINÓ.\n", id);

  }

  MPI_Finalize();

  return 0;
}

/* ------------- ÚTILES ------------- */
int comprobar (int fila, int reinas[], int n) {
  int i;
  for (i=0; i<fila; i++)
  if (  ( reinas[i]==reinas[fila] )                      // Misma columna
  || ( abs(fila-i) == abs(reinas[fila]-reinas[i]) ) ) // Misma diagonal
    return FALSE;
  return TRUE;
}
int colocarReina (int fila, int reinas[], int n) {
  int soluciones = 0;
  if (fila<n) {
    // Quedan reinas por colocar
    for (reinas[fila]=0; reinas[fila]<n; reinas[fila]++) {
      // Comprobamos si la posición
      // de la reina actual es válida
      if (comprobar(fila,reinas,n)) {
        // Si es así, intentamos colocar
        // las reinas restantes
        soluciones += colocarReina (fila+1, reinas, n);
      }
    }
  } else {
    // No quedan reinas por colocar (solución)
    //mostrarTablero(reinas,n);
    //return 1;
    soluciones++;
  }
  return soluciones;
}
/* ------------- ADICIONALES ------------- */
double dwalltime() {

}   //ESTÁ VACÍO























/* ESPACIADO */
