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

long double pi;
int npoints = 10000;
int circle_count;
int points_thread;
int num_threads;
pthread_mutex_t lock;

void* calculate (void *arg){
	pthread_mutex_lock(&lock);
	
	for(j = 1; j < points_thread; j++){
		double n1 = (double)rand() % 2.0;
		double n2 = (double)rand() % 2.0;
		
		if (sqrt(pow(n1, 2) + pow(n2, 2)) <= 1){
			circle_count++;
		}
	}
	
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	if (argc < 2){
		num_threads = 1;
	}	
	else{
		num_threads = atoi(argv[1]);
	}
	
	clock_t start, end;
	double time;
	
	start = clock();
	
	if (pthread_mutex_init(&lock, NULL) != 0){
        	printf("\n mutex init failed\n");
            	return 1;
    	}

	srand((unsigned int)time(NULL));
	pthread_t threads[num_threads];
	points_thread = npoints/num_threads;
	int rc;
	long t;
	for(t = 0;t < num_threads;t++){
		rc = pthread_create(&threads[t], NULL, calculate, NULL);
		if (rc){
		    printf("ERROR; return code from pthread_create() is %d\n", rc);
		    exit(-1);
		}
	}
	
	for(int i = 0; i < num_threads; i++){
		pthread_join(threads[i], NULL);
	}
	
	pi = 4.0 * (double)circle_count/(double)npoints;
	end = clock();
	time = (double)(end - start)/CLOCKS_PER_SEC;
	
	printf("Pi = %f",pi);
	printf("Time with %f threads = %f", num_threads, time);
	pthread_mutex_destroy(&lock);

	return 0;
}
