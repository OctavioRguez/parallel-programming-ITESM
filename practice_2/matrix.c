/*
Autor:
Misael Octavio Rodríguez Macías A01639786

Con apoyo de los siguientes repositorios:

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define n 100

int a[n][n];
int b[n][n];
int c[n][n];

int main(int argc, char** argv){

  int threads_num, solution;
  if (argc < 2){
    threads_num = 1;
  }
  else{
    threads_num = atoi(argv[1]);
  }
  omp_set_num_threads(threads_num);
  printf("Number of threads: %d\n\n", threads_num);

  int i, j, k;
  srand(time(NULL));
  for (i = 0; i < n; i++){
    for (j = 0; j < n; j++){
      a[i][j] = 1 + rand() % 101;
      b[i][j] = 1 + rand() % 101;
      c[i][j] = 0;
    }
  }

#pragma omp parallel for private(i, j, k) shared (a, b, c)
  for (i = 0; i < n;++i){
    for (j = 0; j < n; ++j){
      for (k = 0; k < n; ++k){
        c[i][j] += (a[i][k] * b[k][j]);
      }
    }
  }
  
  if (argc < 3){
    solution = 0;
  }
  else{
    solution = atoi(argv[2]);
  }
  
  if (solution == 1){
    printf("Solution matrix:\n");
    for(i = 0; i < n; i++){
      for(j = 0; j < n; j++){
        printf("%d ", c[i][j]);
      }
      printf("\n");
    }
  }
  return 0;
}
