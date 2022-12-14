/*
Autor:
Misael Octavio Rodríguez Macías A01639786

Con apoyo de los siguientes repositorios:
https://github.com/VictorRodriguez/operating-systems-lecture/blob/master/labs/04/practice1.c
https://github.com/VictorRodriguez/parallel-programming-lecture/blob/main/labs/04/simple-thread.c
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double pi;
int npoints = 10000;
int circle_count;
int points_thread;
int num_threads;
int seed;
pthread_mutex_t lock;

void* calculate (void *arg){
	pthread_mutex_lock(&lock);
	
	for(int j = 1; j < points_thread; j++){
		double n1 = (double)rand_r(&seed)/(double)((unsigned)RAND_MAX + 1);
		double n2 = (double)rand_r(&seed)/(double)((unsigned)RAND_MAX + 1);
		
		if (sqrt(pow(n1, 2) + pow(n2, 2)) <= 1.0){
			circle_count++;
		}
	}
	
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	clock_t start, end;
	double time_used;
	start = clock();
	
	if (argc < 2){
		num_threads = 1;
	}	
	else{
		num_threads = atoi(argv[1]);
	}
	
	if (pthread_mutex_init(&lock, NULL) != 0){
        	printf("\n mutex init failed\n");
            	return 1;
    	}

	seed = time(NULL);
	pthread_t threads[num_threads];
	points_thread = npoints/num_threads;
	int rc;
	for(int t = 0;t < num_threads;t++){
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
	time_used = (double)(end - start)/CLOCKS_PER_SEC;
	
	printf("Pi = %f\n",pi);
	printf("Time with %i threads = %f\n", num_threads, time_used);
	pthread_mutex_destroy(&lock);
	
	return 0;
}
