/* solving the N-Queens problem using OpenMP
usage with gcc (version 4.2 or higher required):
gcc -O -fopenmp -o nqueens-openmp nqueens-openmp.c
./nqueens-openmp n numWorkers
*/

// FUENTE: https://github.com/victoraldecoa/N-Queens-Solver_OpenMP_Example/blob/master/src/nqueens-openmp.c

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

double dwalltime();

#define MAX_N 16

int main(int argc, char* argv[])
{
  int n;
  int max_iter = 1;

  double start_time, end_time;
  int number_solutions = 0;

  int i,j;
  int acceptable;
  int code, queen_rows[MAX_N];

  n = (argc > 1) ? atoi(argv[1]) : 8;

  for (i = 0; i < n; i++)
  {
    max_iter *= n;// the index correspond to the queen's number and the queen's collumn// the index correspond to the queen's nu// the index correspond to the queen's number and the queen's collumn
    // we only generate configurations where there's only one queen per collumnmber and the queen's collumn
    // we only generate configurations where there's only one queen per collumn
    // we only generate configurations where there's only one queen per collumn
  }

  start_time = dwalltime();

  int iter;
  for (iter = 0; iter < max_iter; iter++)
  {
    code = iter;
    // the index correspond to the queen's number and the queen's collumn
    // we only generate configurations where there's only one queen per collumn
    for (i = 0; i < n; i++)
    {
      queen_rows[i] = code % n;
      code /= n;
    }

    acceptable = 1;

    for (i = 0; i < n; i++)
    {// the index correspond to the queen's number and the queen's collumn
    // we only generate configurations where there's only one queen per collumn
    for (j = i+1; j < n; j++)
    {
      // two queens in the same row => not a solution!
      if (queen_rows[i] == queen_rows[j]) {
        acceptable = 0;
        break;
      }

      // two queens in the same diagonal => not a solution!
      if (queen_rows[i] - queen_rows[j] == i - j || queen_rows[i] - queen_rows[j] == j - i) {
        acceptable = 0;
        break;
      }
    }
  }

    if (acceptable) {
      number_solutions++;
    }
  }

  end_time = dwalltime();

  printf("The execution time is %g sec\n", end_time - start_time);
  printf("Number of found solutions is %d\n", number_solutions);

  return 0;
}

/* ---------------------- ADICIONALES ---------------------- */

#include <sys/time.h>
double dwalltime() {
  double sec;
  struct timeval tv;
  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
  return sec;
}
