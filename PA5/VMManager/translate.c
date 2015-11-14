#include "vmmanager.h"

int translate(int laddr, tlb *tlb_cache, page_table *ptable, phys_mem *pmem){
	int paddr;
	int page_num, frame_num, offset;
	int tlb_index;
	
	offset = getOfs(laddr);
	page_num = getPageNum(laddr);
	
	if((tlb_index = isInTLB(tlb_cache, page_num)) >= 0){
		//tlb_cache->hits++;
		frame_num = getFrameFromTLB(tlb_cache + tlb_index);
		updateTLBTimestamp(tlb_cache);
		(tlb_cache + tlb_index)->timestamp = 1;
	}
	else{
		//tlb_cache->misses++;
		if(!isInPageTable(ptable + page_num)){
			ptable->page_faults++;
			getPageFromBS(pmem, ptable, page_num);
		}
		
		frame_num = getFrameFrompTable(ptable + page_num);
		updateTLB(tlb_cache, page_num, frame_num);
	}
	
	paddr = (frame_num << OFS_BITS) + offset;
	return paddr;
}

void getPageFromBS(phys_mem *pmem, page_table *ptable, int page_num){
	int frame_num = getFrameToReplace(pmem);
	FILE *fp = fopen(BACKING_STORE, "rb");
	if(!fp){
		fprintf(stderr, "Error: Cannot open file %s\n", BACKING_STORE);
		exit(-1);
	}
	fseek(fp, page_num * PAGE_SIZE, SEEK_SET);
	fread((pmem + frame_num)->frame, 1, PAGE_SIZE, fp);
	updatePageTable(ptable + page_num, frame_num);
	updateFrameToReplace(pmem);
	
	fclose(fp);
}

int getPageNum(int addr){
	return (addr >> OFS_BITS) & ((1 << PAGE_BITS) - 1);
}

int getFrameNum(int addr){
	return (addr >> OFS_BITS) & ((1 << FRAME_BITS) - 1);
}

int getOfs(int addr){
	return addr & ((1 << OFS_BITS) - 1);
}

int getFrameToReplace(phys_mem *pmem){
	return pmem->next_frame_to_replace;
}

int getFrameFrompTable(page_table *ptable){
	return ptable->frame_num;
}

int getFrameFromTLB(tlb *tlb_cache){
	return tlb_cache->frame_num;
}

char getByte(phys_mem *pmem, int paddr){
	int offset = getOfs(paddr);
	int frame_num = getFrameNum(paddr);
	return (pmem + frame_num)->frame[offset];
}

int isInTLB(tlb *tlb_cache, int page_num){
	int i;
	for(i=0; i<TLB_ENTRIES; i++){
		if((tlb_cache + i)->page_num == page_num){
			if((tlb_cache + i)->valid == 0){
				tlb_cache->misses++;
				return -1;
			}
			else{
				tlb_cache->hits++;
				return i;
			}
		}
	}
	tlb_cache->misses++;
	return -1;
}

int isInPageTable(page_table *ptable){
	return (ptable->valid == 1) ? 1 : 0;
}