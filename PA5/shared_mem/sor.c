#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <math.h>

#define N 3 // between 3 and 6 inclusive
#define NAME "MATRIX"
#define CONV_FACTOR 0.001

#if N==3
const int A[N][N] = {{-2,1,0},{1,-2,1},{0,1,-2}};
const int b[N] = {-1, 0, -1};
#endif

#if N==4
const int A[N][N] = {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};
const int b[N] = {4,4,4,4};
#endif

#if N==5
const double A[N][N] = {{1,1,1,1,1},{-2,-2,-2,-2,-2},{5,5,5,5,5},{15,15,15,15,15},{-1.5,-1.5,-1.5,-1.5,-1.5}};
const int b[N] = {100,-200,500,1500,-150};
#endif

#if N==6
const double A[N][N] = {{2,3,4,5,6,7},{4,6,8,10,12,14},{20,30,40,50,60,70},{-2,-3,-4,-5,-6,-7},{.2,.3,.4,.5,.6,.7},{-.2,-.3,-.4,-.5,-.6,-.7}}; 
const double b[N] = {215,430,2150,-215,21.5,-21.5}; 
#endif

void calc_sor(double, int, void *);
void init_x(void *);
void print_x(void *);
int conv(void *ptr, double a[]);
void print_A(void);
void print_b(void);

int main(int argc, char **argv){
	
	int pid[N];
	int status;
	int shm_fd;
	void *x;
	int i;
	int hasConv = 0;
	double previter[N];
	
	double omega = 1.0;
	
	for(i=0; i<N; i++){
		previter[i] = 0.0;
	}
	
	shm_fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, N * sizeof(int));
	x = mmap(0, N * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	
	init_x(x);
	
	print_A();
	print_b();
	
	while(hasConv == 0){
		for(i=0; i<N; i++){
			if((pid[i] = fork()) < 0){
				fprintf(stderr, "Error: fork failed\n");
				exit(-1);
			}
			else if(pid[i] == 0){
				// is child process
				calc_sor(omega, i, x);
				exit(0);
			}
			
			// is parent process
			wait(&status);
		}
		
		// check for convergence
		hasConv = conv(x, previter);
		for(i=0; i<N; i++){
			previter[i] = *(double *)(x + i*sizeof(double));
		}
		
	}
	
	printf("x:\n");
	print_x(x);
	
	shm_unlink(NAME);
	return 0;
	
}

void calc_sor(double omega, int i, void *ptr){
	double phi = *(double *)(ptr + i*sizeof(double)); // initial guess to solution
	double phi_j;
	double sigma;
	int j;
	
	sigma = 0;
	for(j=0; j<N; j++){
		if(j != i){
			phi_j = *(double *)(ptr + j*sizeof(double));
			sigma = sigma + A[i][j] * phi_j;
		}
	}
	phi = (1 - omega)*phi + (omega/A[i][i])*(b[i] - sigma);
	*(double *)(ptr + i*sizeof(double)) = phi;
}

void init_x(void *ptr){
	int i;
	for(i=0; i<N; i++){
		*(double *)(ptr + i * sizeof(double)) = 0.0;
	}
}

void print_x(void *ptr){
	int i;
	for(i=0; i<N; i++){
		printf("%.3f \n", *(double *)(ptr + i * sizeof(double)));
	}
	printf("\n");
}

int conv(void *ptr, double a[]){
	int i;
	int hasConv = 0;
	for(i=0; i<N; i++){
		if(fabs(*(double *)(ptr + i*sizeof(double)) - a[i]) <= CONV_FACTOR){
			hasConv = 1;
		}
	}
	return hasConv;
}

void print_A(void){
	int i,j;
	printf("A: \n");
	for(j=0; j<N; j++){
		for(i=0; i<N; i++){
			printf("%.3f\t", (double)A[j][i]);
		}
		printf("\n");
	}
	printf("\n");
}

void print_b(void){
	int i;
	printf("b: \n");
	
	for(i=0; i<N; i++){
		printf("%.3f\n", (double)b[i]);
	}
	printf("\n");
}