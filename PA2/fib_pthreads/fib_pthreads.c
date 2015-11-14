/*
 * To generate the Fibonacci sequence of length given by program argument 
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

long *fib_array;

void *generate(void *);
long fib(long);

int main(int argc, char **argv){
	
	pthread_t t;
	long seq_len, i;
	
	if(argc != 2){
		fprintf(stderr, "Usage: ./fib_pthreads num_fib_numbers\n");
		exit(-1);
	}
	
	seq_len = (long)atoi(argv[1]);
	fib_array = (long *)malloc(seq_len * sizeof(long));
	pthread_create(&t, NULL, generate, (void*)&seq_len);
	pthread_join(t, NULL);
	
	printf("Fibonacci sequence of length %li is: \n", seq_len);
	for(i=0; i<seq_len; i++){
		printf("%li, ", fib_array[i]);
	}
	printf("\n");
	
	free(fib_array);
	return 0;
}

void *generate(void *params){
	long seq_len = *(long *)params;
	long i;
	for(i=0; i<seq_len; i++){
		fib_array[i] = fib(i);
	}
	pthread_exit((void *)0);
}

long fib(long n){
	if(n == 0) return 0;
	if(n == 1) return 1;
	return fib(n-1) + fib(n-2);
}