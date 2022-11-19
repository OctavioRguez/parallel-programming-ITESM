/*
Autor:
Misael Octavio Rodríguez Macías A01639786

Con apoyo de los siguientes repositorios:
https://github.com/VictorRodriguez/parallel-programming-lecture/blob/main/labs/04/simple-thread.c
https://github.com/VictorRodriguez/parallel-programming-lecture/blob/main/labs/04/mutex-thread.c
https://github.com/VictorRodriguez/parallel-programming-lecture/blob/main/labs/07/omp_for.c
https://levelup.gitconnected.com/solving-2d-heat-equation-numerically-using-python-3334004aa01a
*/

#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#define row_num 6
#define col_num 7

//pthread_mutex_t lock;
bool kill = false; //Kill all the threads when true
int TemperatureMatrix[row_num][col_num];
char HeatMatrix[row_num][col_num]={
{'C','C','C','C','C','C','C'},
{'C','C','C','C','C','C','C'},
{'C','C','H','H','H','C','C'},
{'C','C','H','H','H','C','C'},
{'C','C','H','H','H','C','C'},
{'C','C','C','C','C','C','C'}};


//Check if the threads have already completed the Temperature Matrix
void checkMatrix(){
  for(int a = 0; a < row_num; a++){
    for(int b = 0; b < col_num; b++){
      if (TemperatureMatrix[a][b] == 0){
        return;
      }
    }
  }
  kill = true; //Kill all the threads
}

//Move threads
void moveThread(int* i, int* j){
  //Generate a random number (-1, 0 or 1)
  int limit1x = -1;
  int limit2x = -1;
  int limit1y = -1;
  int limit2y = -1;
  if (*i == 0){
    //Generate a random number (0 or 1)
    limit1x = 0;
    limit2x = 0;
  }
  else if(*i == row_num - 1){
    //Generate a random number (0 or -1)
    limit2x = 0;
  }

  if (*j == 0){
    //Generate a random number (0 or 1)
    limit1y = 0;
    limit2y = 0;
  }
  else if(*j == col_num - 1){
    //Generate a random number (0 or -1)
    limit2y = 0;
  }

  int x = (limit1x) + rand() % (2 - (limit2x));
  int y = (limit1y) + rand() % (2 - (limit2y));
  *i += x;
  *j += y;
}

//Set temperature
void* temperature(void* id){
  int i = rand() % row_num;
  int j = rand() % col_num;

  //Keep moving until the Temperature Matrix is completed
  while(true){
    if (id == 0){ //Only the first thread check the Temperature Matrix
      checkMatrix();
    }

    if (kill){ //Stop
      pthread_exit(NULL);
    }

    moveThread(&i, &j); //Call the function to move the thread

    //Generate a random number in the Temperature Matrix
    //pthread_mutex_lock(&lock);
    if (HeatMatrix[i][j] == 'C'){
      TemperatureMatrix[i][j] = rand() % 61;
    }
    else{
      TemperatureMatrix[i][j] = 61 + rand() % (101 - 61);
    }
    //pthread_mutex_unlock(&lock);
  }
}

int main(int argc, char *argv[]){

  //Print Char Matrix
  printf("Heat Matrix:\n");
  for(int row = 0; row < row_num; row++){
    for(int col = 0; col < col_num; col++){
      printf("%c ", HeatMatrix[row][col]);
      TemperatureMatrix[row][col] = 0;
    }
    printf("\n");
  }

  //Define number of threads
  int num_threads = 4;
  if (argc > 1){
    num_threads = atoi(argv[1]);
  }
  pthread_t threads[num_threads];

  //Create threads
  int rc;
  srand(time(NULL));
  for(long k = 0; k < num_threads; k++){
      rc = pthread_create(&threads[k], NULL, temperature, (void*) k);
      if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
      }
  }

  //Print Temperature Matrix
  printf("\nTemperature Matrix:\n");
  for(int row = 0; row < row_num; row++){
    for(int col = 0; col < col_num; col++){
      printf("%d ", TemperatureMatrix[row][col]);
    }
    printf("\n");
  }

  //Heat Equation values
  int max_iter_time = 80;
  int alpha = 2;
  int delta_x = 1;
  float delta_t = (pow(delta_x, 2))/(4 * alpha);

  //Solve heat equation
  float gamma = (alpha * delta_t) / (pow(delta_x, 2));
  int u[max_iter_time][row_num][col_num];
  omp_set_num_threads(num_threads);

  int k, i, j;
#pragma omp parallel for private(k, i, j) shared (u, TemperatureMatrix)
  for(k = 0; k < max_iter_time; k++){
    for(i = 0; i < row_num; i++){
      for(j = 0; j < col_num; j++){
        u[k][i][j] = TemperatureMatrix[i][j];
      }
    }
  }

#pragma omp parallel for private(k, i, j) shared (u)
  for(k = 0; k < max_iter_time - 1; k++){
    for(i = 1; i < row_num - 1; i+= delta_x){
      for(j = 1; j < col_num - 1; j+= delta_x){
        u[k + 1][i][j] = gamma * (u[k][i+1][j] + u[k][i-1][j] + u[k][i][j+1] + u[k][i][j-1] - 4*u[k][i][j]) + u[k][i][j];
      }
    }
  }

  printf("\nDear manager:");
  printf("\nThis is the heat equation for the factory:");
  printf("\ny = %d * (%.3f / (%d)^2)\n", alpha, delta_t, delta_x);
  printf("\nThis is the forecast of the heat at the factory for the incoming 10 hours:");
  printf("\nCode for generate the heat map (needs the values from the random temperature matrix): https://github.com/OctavioRguez/parallel-programming-ITESM/blob/main/practice_3/Heat_Map.ipynb");
  printf("\n");

  //pthread_mutex_destroy(&lock);
  return 0;
}
