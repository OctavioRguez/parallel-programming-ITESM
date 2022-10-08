/*
Autor:
Misael Octavio Rodríguez Macías A01639786

Con apoyo de los siguientes repositorios:
https://github.com/VictorRodriguez/operating-systems-lecture/blob/master/labs/04/practice1.c
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

long double global_pi = 0;
pthread_mutex_t lock;

void* calculate (void *arg){
	int npoints = 10000;
	int circle_count = 0;
	pthread_mutex_lock(&lock);
	
	for(j = 1; j < npoints; j++){
		srand((unsigned int)time(NULL));
		int n1 = rand() % 2;
		srand((unsigned int)time(NULL));
		int n2 = rand() % 2; 
		
		if (sqrt(pow(n1, 2) + pow(n2, 2)) < 1){
			circle_count++;
		}
	}
	global_pi += 4.0 * circle_count/npoints;
	
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	clock_t start, end;
	double time;
	
	start = clock();
	
	if (pthread_mutex_init(&lock, NULL) != 0){
        	printf("\n mutex init failed\n");
            	return 1;
    	}

	pthread_t threads[argc];
	int rc;
	long t;
	for(t = 0;t < argc;t++){
		rc = pthread_create(&threads[t], NULL, calculate, NULL);
		if (rc){
		    printf("ERROR; return code from pthread_create() is %d\n", rc);
		    exit(-1);
		}
	}
	
	pthread_mutex_destroy(&lock);
	printf("%d\n", global_pi);

	return 0;
}
