#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t lock;
bool kill = false; //Kill all the threads when true
int TemperatureMatrix[6][7];
char HeatMatrix[6][7]={
{'C','C','C','C','C','C','C'},
{'C','C','C','C','C','C','C'},
{'C','C','H','H','H','C','C'},
{'C','C','H','H','H','C','C'},
{'C','C','H','H','H','C','C'},
{'C','C','C','C','C','C','C'}};


//Check if the threads have already completed the Temperature Matrix
void checkMatrix(){
  for(int a = 0; a < 6; a++){
    for(int b = 0; b < 7; b++){
      if (TemperatureMatrix[a][b] == -1){
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
  else if(*i == 5){
    //Generate a random number (0 or -1)
    limit2x = 0;
  }

  if (*j == 0){
    //Generate a random number (0 or 1)
    limit1y = 0;
    limit2y = 0;
  }
  else if(*j == 6){
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
  int i = rand() % 6;
  int j = rand() % 7;

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
    pthread_mutex_lock(&lock);
    if (HeatMatrix[i][j] == 'C'){
      TemperatureMatrix[i][j] = rand() % 61;
    }
    else{
      TemperatureMatrix[i][j] = 61 + rand() % (101 - 61);
    }
    pthread_mutex_unlock(&lock);
  }
}

int main(int argc, char *argv[]){

  //Print Char Matrix
  printf("Heat Matrix:\n");
  for(int row = 0; row < 6; row++){
    for(int col = 0; col < 7; col++){
      printf("%c ", HeatMatrix[row][col]);
      TemperatureMatrix[row][col] = -1;
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
  for(int row = 0; row < 6; row++){
    for(int col = 0; col < 7; col++){
      printf("%d ", TemperatureMatrix[row][col]);
    }
    printf("\n");
  }

  //Heat Equation
  #pragma omp parallel


  printf("\nDear manager:");
  printf("\nThis is the heat equation for the factory:");
  //HeatEquation
  printf("\nThis is the forecast of the heat at the factory for the incoming 10 hours:");
  //Plot
  printf("\nWe must turn on fans in <X> amount of hours to avoid arriving to Max temperature (70 °C average) in the factory.\n");
  pthread_mutex_destroy(&lock);
  
  return 0;
}
