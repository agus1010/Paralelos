#include<stdio.h>
#include<stdlib.h>

int N=100;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

void imprimeVector(double *S, int sizeMatrix){
  int i;
  printf("\n ");
  for(i=0 ;i<sizeMatrix;i++)
	printf(" %f " ,S[i]);

  printf("\n\n ");
}

int main(int argc,char* argv[]){

 double *A,*BT,*C;
 int i,j,k;
 int check = 1;
 double timetick;

 //Controla los argumentos al programa
 if ((argc != 2) || ((N = atoi(argv[1])) <= 0) ) {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    exit(1);
  }

 //Aloca memoria para las matrices
 A=(double*)malloc(sizeof(double)*N*N);
 BT=(double*)malloc(sizeof(double)*N*N);
 C=(double*)malloc(sizeof(double)*N*N);
 //Inicializa las matrices
 //La matriz A se inicializa todas las columnas en 0 menos la ultima
 //La matriz BT se inicializa triangular inferior con 1
 //Esto permite que el resultado sean todos 1
 //La matriz C se inicializa en 0
 for(i=0;i<N;i++){
  for(j=0;j<N;j++){

   if(j<N-1){
    A[i*N+j]=0;
   }else{
    A[i*N+j]=1;
   }

   if(i>=j){
    BT[i+N*j]=N;
   }else{
    BT[i+N*j]=0;
   }

   C[i*N+j]=0;
  }
 }

 //Realiza la multiplicacion
 /*
   Este código realiza la multiplicación de "atrás hacia adelante".
   Recorre primero BT, mientras no encuentre un 0 realizar la multiplicación con
   A y sumar a un total. Esto es válido porque BT es una matriz triangular
   inferior; si fuera triangular superior: recorrer normal y parar cuando se
   encuentre un 0.
   Además, la disposición del acceso a las matrices permite un mejor uso de la
   localidad de las referencias en caché, ahorrando accesos innecesarios.
 */
 int bt;
 int resultado;
 int indice;
 timetick = dwalltime();
 for(i=N-1;i>=0;i--){
   for(j=N-1;j>=0;j--){
     resultado = 0;
     indice = N-1;
     bt = BT[indice+j*N];
     while(bt!=0 & indice>=0) {
       resultado = resultado + bt * A[indice+i*N];
       indice--;
       bt = BT[indice+j*N];
     }
     C[i*N+j] = resultado;
   }
 }

 printf("Tiempo en segundos %f\n", dwalltime() - timetick);

 //Verifica el resultado
 for(i=0;i<N;i++){
  for(j=0;j<N;j++){
   check = check && (C[i*N+j]==N);
  }
 }

 if(check){
  printf("Multiplicacion de matriz triangular correcta\n");
 }else{
  printf("Multiplicacion de matriz triangular erroneo\n");
 }


     /*printf("\n\n  A (como esta almacenada): \n" );
    imprimeVector(A, N*N);
     printf("\n\n  BT (como esta almacenada): \n" );
    imprimeVector(BT,N*N);
    printf("\n\n  C (como esta almacenada): \n" );
    imprimeVector(C,N*N);*/

 free(A);
 free(BT);
 free(C);

 return(0);
}
