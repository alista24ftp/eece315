#include <stdio.h>
#include <stdlib.h>

#define PAGE_ENTRIES 256
#define PAGE_SIZE 256
#define PAGE_BITS 8

#define FRAME_SIZE 256
#define FRAMES 256
#define FRAME_BITS 8

#define BACKING_STORE "BACKING_STORE.bin"
#define CORRECT "correct.txt"

#define OFS_BITS 8
#define TLB_ENTRIES 16

// structs:
typedef struct{
	int valid;
	int frame_num;
	int page_faults;
}page_table;

typedef struct{
	char frame[FRAME_SIZE];
	int next_frame_to_replace;
}phys_mem;

typedef struct{
	int page_num;
	int frame_num;
	int hits;
	int misses;
	int timestamp;
	int valid;
}tlb;

// function prototypes:
void init_ptable(page_table *);
void init_pmem(phys_mem *);
void init_tlb(tlb *);

int translate(int, tlb *, page_table *, phys_mem *);
int getPageNum(int);
int getFrameNum(int);
int getOfs(int);
void getPageFromBS(phys_mem *, page_table *, int);
int getFrameToReplace(phys_mem *);
int getFrameFrompTable(page_table *);
int getFrameFromTLB(tlb *);
int isInTLB(tlb *, int);
int isInPageTable(page_table *);
char getByte(phys_mem *, int);

void updateTLB(tlb *, int, int);
void updateTLBTimestamp(tlb *);
int tlbFull(tlb *);
int getEvictIndex(tlb *);
int getNextTLBIndex(tlb *);
void updatePageTable(page_table *, int);
void updateFrameToReplace(phys_mem *);
