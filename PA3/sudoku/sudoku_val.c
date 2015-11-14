#include "sudoku_puzzle.h"

int puzzle[NUM_ROWS][NUM_COLS];
int valid[NUM_THREADS];

int main(int argc, char **argv){
	
	pthread_t r, c;
	pthread_t g[NUM_GRIDS];
	int puzzle_num;
	int i;
	int isValid = 1; // set by default
	
	if(argc != 2){
		fprintf(stderr, "Usage: ./sudoku_val <puzzle_number>\n");
		exit(-1);
	}
	puzzle_num = atoi(argv[1]);
	if(puzzle_num < 0 || puzzle_num > 3){
		fprintf(stderr, "Error: <puzzle_number> must be between 0 and 3 inclusive\n");
		exit(-1);
	}
	
	select_puzzle(puzzle_num);
	init_valid_array();
	
	parameters *data;
	parameters **dptr;
	allocParams(&data, &dptr);
	
	pthread_create(&r, NULL, check_rows, (void *)data);
	pthread_create(&c, NULL, check_cols, (void *)data);
	for(i=0; i<NUM_GRIDS; i++){
		pthread_create(&g[i], NULL, check_grids, (void *)dptr[i]);
	}
	
	pthread_join(r, NULL);
	pthread_join(c, NULL);
	for(i=0; i<NUM_GRIDS; i++){
		pthread_join(g[i], NULL);
	}
	
	// check valid array
	for(i=0; i<NUM_THREADS; i++){
		if(valid[i] == 0){
			isValid = 0;
			break;
		}
	}
	
	if(!isValid){
		printf("This puzzle is invalid\n");
	}
	else{
		printf("This puzzle is valid\n");
	}
	
	// free all malloc's
	for(i=0; i<NUM_GRIDS; i++){
		free(dptr[i]);
	}
	free(dptr);
	free(data);
	return 0;
	
}

void init_valid_array(void){
	int i;
	for(i=0; i<NUM_THREADS; i++){
		valid[i] = 0;
	}
}

void select_puzzle(int puzzle_num){
	switch(puzzle_num){
		case 0:
			init_puzzle(valid_puzzle0);
			break;
			
		case 1:
			init_puzzle(invalid_puzzle0);
			break;
			
		default:
			init_puzzle(empty_puzzle);
	}
		
}

void init_puzzle(int puzzle_type[][NUM_COLS]){
	int i, j;
	for(j=0; j<NUM_ROWS; j++){
		for(i=0; i<NUM_COLS; i++){
			puzzle[j][i] = puzzle_type[j][i];
		}
	}
}

void allocParams(parameters **a, parameters ***b){
	int i, j;
	int k = 0;
	parameters *data;
	parameters **dptr;
	data = (parameters *)malloc(sizeof(parameters));
	dptr = (parameters **)malloc(NUM_GRIDS * sizeof(parameters *));
	for(i=0; i<NUM_GRIDS; i++){
		dptr[i] = (parameters *)malloc(sizeof(parameters));
	}
	
	// initialize data
	data->row = 1;
	data->col = 1;
	
	// initialize dptr
	for(j=0; j<SUBGRID_SIZE; j++){
		for(i=0; i<SUBGRID_SIZE; i++){
			dptr[k]->row = (j * SUBGRID_SIZE) + 1;
			dptr[k]->col = (i * SUBGRID_SIZE) + 1;
			k++;
		}
	}
	
	*a = data;
	*b = dptr;
	
}

void *check_rows(void *param){
	parameters *p = (parameters *)param;
	int r = p->row - 1;
	int c = p->col - 1;
	int row_valid[NUM_ROWS];
	int cur_row[NUM_COLS];
	
	while(r<NUM_ROWS){
		for(c=p->col - 1; c<NUM_COLS; c++){
			cur_row[c] = puzzle[r][c];
		}
		if((row_valid[r] = validate(cur_row)) == 0){
			valid[0] = 0;
			break;
		}
		else{
			valid[0] = 1;
		}
		r++;
	}
	
	pthread_exit((void *)0);
}

void *check_cols(void *param){
	parameters *p = (parameters *)param;
	
	int r = p->row - 1;
	int c = p->col - 1;
	int col_valid[NUM_COLS];
	int cur_col[NUM_ROWS];
	
	while(c<NUM_COLS){
		for(r=p->row - 1; r<NUM_ROWS; r++){
			cur_col[r] = puzzle[r][c];
		}
		if((col_valid[c] = validate(cur_col)) == 0){
			valid[1] = 0;
			break;
		}
		else{
			valid[1] = 1;
		}
		c++;
	}
	
	pthread_exit((void *)0);
}

void *check_grids(void *param){
	parameters *p = (parameters *)param;
	int r = p->row - 1;
	int c = p->col - 1;
	int r_end = r + SUBGRID_SIZE;
	int c_end = c + SUBGRID_SIZE;
	int grid_contents[SUBGRID_SIZE * SUBGRID_SIZE];
	int index;
	int k=0;
	
	if(c == SUBGRID_SIZE){
		index = r + c;
	}
	else if(c == 0){
		index = r + 2;
	}
	else{
		index = r + c - 2;
	}
	
	for(r = p->row-1; r < r_end; r++){
		for(c = p->col-1; c < c_end; c++){
			grid_contents[k] = puzzle[r][c];
			k++;
		}
	}
	
	valid[index] = validate(grid_contents);
	
	pthread_exit((void *)0);
}

int validate(int array[]){
	int i, j;
	
	// first check if all numbers in array is between 1 and 9 inclusive
	for(i=0; i<NUM_ROWS; i++){
		if(array[i] > UPPER_NUM || array[i] < LOWER_NUM){
			return 0; 
		}
	}
	
	// check for repeats
	for(i=0; i<NUM_ROWS; i++){
		for(j=i+1; j<NUM_ROWS; j++){
			if(array[j] == array[i]){
				return 0;
			}
		}
	}
	
	// if none of the above returns, that means the array is valid
	return 1;
}