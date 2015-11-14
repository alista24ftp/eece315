#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 9
#define NUM_COLS 9
#define NUM_GRIDS 9
#define SUBGRID_SIZE 3
#define NUM_THREADS 11
#define UPPER_NUM 9
#define LOWER_NUM 1

// structure for passing data to threads
typedef struct{
	int row;
	int col;
}parameters;

const int valid_puzzle0[NUM_ROWS][NUM_COLS] = {{6,2,4,5,3,9,1,8,7},
											   {5,1,9,7,2,8,6,3,4},
											   {8,3,7,6,1,4,2,9,5},
											   {1,4,3,8,6,5,7,2,9},
											   {9,5,8,2,4,7,3,6,1},
											   {7,6,2,3,9,1,4,5,8},
											   {3,7,1,9,5,6,8,4,2},
											   {4,9,6,1,8,2,5,7,3},
											   {2,8,5,4,7,3,9,1,6}};
											   
const int invalid_puzzle0[NUM_ROWS][NUM_COLS] = {{6,2,4,5,3,9,1,8,7},
											     {5,1,9,7,2,8,6,3,4},
											     {8,3,7,6,1,4,2,9,5},
											     {1,4,3,8,6,5,7,2,9},
											     {9,5,8,2,4,7,3,6,1},
											     {7,6,2,3,9,1,4,5,8},
											     {3,7,1,9,5,6,8,4,2},
											     {4,9,6,1,8,2,5,7,3},
											     {2,8,5,4,7,3,9,6,6}};

const int empty_puzzle[NUM_ROWS][NUM_COLS] = {{0,0,0,0,0,0,0,0,0},
											  {0,0,0,0,0,0,0,0,0},
											  {0,0,0,0,0,0,0,0,0},
											  {0,0,0,0,0,0,0,0,0},
											  {0,0,0,0,0,0,0,0,0},
											  {0,0,0,0,0,0,0,0,0},
											  {0,0,0,0,0,0,0,0,0},
											  {0,0,0,0,0,0,0,0,0},
											  {0,0,0,0,0,0,0,0,0}};
											   
// FUNCTION PROTOTYPES:
void select_puzzle(int);
void init_valid_array(void);
void init_puzzle(int puzzle_type[][NUM_COLS]);
// void allocParams(parameters *, parameters **);
void allocParams(parameters **, parameters ***);

void *check_rows(void *);
void *check_cols(void *);
void *check_grids(void *);
int validate(int array[]);

