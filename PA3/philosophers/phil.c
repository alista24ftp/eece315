#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NAPTIME 2000000
#define ONE_SEC 1000000 // representing 1000000 usecs (which is 1 sec)
#define NPHILOSOPHERS 5
#define NMEALS 3
#define VERBOSE 1

typedef struct philosophers {
	int id;
} philosopher;

// I defined the struct to be "chopstick" rather than "fork" since the latter gave me compilation errors
typedef struct chopsticks {
	int isFree;
	pthread_mutex_t lock;// = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t forkcond; // not really used for this version of the assignment
} chopstick;

/*
 * Global variables representing the forks and philosophers that will be used by the threads
 */
chopstick f[NPHILOSOPHERS];
philosopher phil[NPHILOSOPHERS];

/*
 * At the beginning of program execution, initialize all forks to be free
 */
void init(void){
	int i;
	for(i=0; i<NPHILOSOPHERS; i++){
		f[i].isFree = 1; //1 - free, 0 - used
		f[i].lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
		f[i].forkcond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
		phil[i].id = i;
	}
}

/*
 * Returns the index of the left fork corresponding to the philosopher
 * Requires: philosopher p
 * Returns: id of p (representing index of left fork of p)
 */
int indexOfLfork(philosopher p){
	return p.id;
}

/*
 * Returns the index of the right fork corresponding to the philosopher
 * Requires: philosopher p
 * Returns: (id of p) plus 1, or 0 when id of p == NPHILOSOPHERS - 1 (representing index of right fork of p)
 */
int indexOfRfork(philosopher p){
	return (p.id + 1) % NPHILOSOPHERS;
}

/*
 * Delay time between 1 to 3 seconds, and can represent both eating and thinking
 */
void takeNap(void){
	usleep((random() % NAPTIME) + ONE_SEC);
}

/*
 * Picking up the fork corresponding with the index (ie. setting f[index].isFree to 0)
 * The philosopher will try to pick up the fork by first checking f[index].isFree 
 * Requires: index of fork to be picked up
 * Returns: 1 - f[index].isFree == 0, meaning that someone else is holding onto the fork (philosopher must wait)
 * 			0 - no one else is holding onto the fork, and thus the fork is free to be picked up (no need to wait)
 */
int pickup(int index){
	pthread_mutex_lock(&(f[index].lock));
	if(!f[index].isFree){
		pthread_mutex_unlock(&(f[index].lock));
		return 1;
	}
	f[index].isFree = 0; //no longer free to use
	pthread_mutex_unlock(&(f[index].lock));
	return 0;
}

/*
 * Putting down the fork by setting f[index].isFree to 1
 * Requires: index of fork to be put down
 */
void putdown(int index){
	pthread_mutex_lock(&(f[index].lock));
	f[index].isFree = 1; //now free to use
	pthread_mutex_unlock(&(f[index].lock));
	
}

/*
 * The activity of each individual philosopher
 * Description: This philosopher will pick up forks in a random order (either left-right or right-left) every meal,
 * 				and will start over from the beginning (thus putting down any forks that he picked up) if he has to
 *				wait for forks to be ready (indicated by return value of pickup(): 1 - must wait, 0 - no need to wait).
 * 				Also, this philosopher will alternate between thinking and eating (where takeNap() is invoked). Once
 * 				the philosopher finishes NMEALS, he will exit
 * Requires: args - represent philosopher id passed in through pthread_create()
 * Returns: NULL
 */
void *activity(void *args){
	int i;
	philosopher p;
	p.id = *(int *)args;
	int randfork[2] = {indexOfLfork(p), indexOfRfork(p)};
	int randnum, fork1, fork2;

	for(i=0; i<NMEALS; i++) {

		randnum = rand() % 2;
		fork1 = randfork[randnum];
		fork2 = randfork[(randnum + 1) % 2];
		
		takeNap();
		if(pickup(fork1) > 0){
			i--;
			continue;
		}
#if VERBOSE==1
		printf("philosopher %d picked up fork %d\n", p.id, fork1);
#endif
		//takeNap();
		if(pickup(fork2) > 0){
			putdown(fork1);
			printf("philosopher %d gives up and tries again...\n", p.id);
			i--;
			continue;
		}
#if VERBOSE == 1
		printf("philosopher %d picked up fork %d\n", p.id, fork2);
		printf("philosopher %d eats\n", p.id);
#endif
		takeNap();
		putdown(fork1);
		putdown(fork2);
#if VERBOSE == 1
		printf("philosopher %d put down both forks and finishes meal %d\n", p.id, i);
#endif
	}

#if VERBOSE == 1
	printf("philosopher %d exits\n", p.id);
#endif
	pthread_exit((void *)0);
}

/*
 * Initializing, creating, and joining threads
 */
int main(int argc, char **argv){
	int i;
	pthread_t threads[NPHILOSOPHERS];

	srandom(time(0));
	init();
	
	for(i=0; i<NPHILOSOPHERS; i++){
		pthread_create(&threads[i], NULL, activity, &(phil[i].id));
	}

	for(i=0; i<NPHILOSOPHERS; i++){
		pthread_join(threads[i], NULL);
	}
	
	return 0;
}
