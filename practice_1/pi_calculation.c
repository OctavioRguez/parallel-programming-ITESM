
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

long double global_pi = 0;
pthread_mutex_t lock;

void* calculate (void *arg){
	pthread_mutex_lock(&lock);
	
	npoints = 10000;
	circle_count = 0;
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
}


void *PrintHello(void *threadid){
	long tid;
	tid = (long)threadid;
	printf("Hello World! It's me, thread #%ld!\n", tid);
	global_variable++;
	printf("Global variable %d\n",global_variable);
	
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	pthread_t threads[argc];
	
	if (pthread_mutex_init(&lock, NULL) != 0){
        	printf("\n mutex init failed\n");
            	return 1;
    	}

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
