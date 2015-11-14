#include "vmmanager.h"

int main(int argc, char **argv){
	FILE *addresses;
	int laddr;
    int	paddr;
	int num_translated = 0;
	page_table *ptable = (page_table *)malloc(PAGE_ENTRIES * sizeof(page_table));
	phys_mem *pmem = (phys_mem *)malloc(FRAMES * sizeof(phys_mem));
	tlb *tlb_cache = (tlb *)malloc(TLB_ENTRIES * sizeof(tlb));
	
	if(argc != 2){
		fprintf(stderr, "Usage: ./vmmanager <filename>\n");
		exit(-1);
	}
	
	addresses = fopen(argv[1], "r");
	if(!addresses){
		fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
		exit(-1);
	}
	
	init_ptable(ptable);
	init_pmem(pmem);
	init_tlb(tlb_cache);
	
	fseek(addresses, 0, SEEK_SET);
	while(fscanf(addresses, "%d", &laddr) == 1){
		printf("Logical: %d\t", laddr);
		paddr = translate(laddr, tlb_cache, ptable, pmem);
		printf("Physical: %d\t", paddr);
		num_translated++;
		printf("Byte: %d\n", (int)getByte(pmem, paddr));
	}
	
	printf("Total addresses translated: %d\n", num_translated);
	printf("Total page faults: %d\n", ptable->page_faults);
	printf("Total page fault rate: %.3f\n", ptable->page_faults / (double)num_translated);
	printf("Total TLB hits: %d\n", tlb_cache->hits);
	printf("Total TLB misses: %d\n", tlb_cache->misses);
	fclose(addresses);
	
	free(tlb_cache);
	free(pmem);
	free(ptable);
	return 0;
}