#include "vmmanager.h"

void init_ptable(page_table *ptable){
	ptable->page_faults = 0;
	int i;
	for(i=0; i<PAGE_ENTRIES; i++){
		(ptable + i)->valid = 0;
		(ptable + i)->frame_num = 0;
	}
}

void init_pmem(phys_mem *pmem){
	pmem->next_frame_to_replace = 0;
}

void init_tlb(tlb *tlb_cache){
	tlb_cache->hits = 0;
	tlb_cache->misses = 0;
	int i;
	for(i=0; i<TLB_ENTRIES; i++){
		(tlb_cache + i)->page_num = 0;
		(tlb_cache + i)->frame_num = 0;
		(tlb_cache + i)->valid = 0;
		(tlb_cache + i)->timestamp = 0;
	}
}

void updateTLB(tlb *tlb_cache, int page_num, int frame_num){
	int index_evict, next;
	if(tlbFull(tlb_cache)){
		index_evict = getEvictIndex(tlb_cache);
		(tlb_cache + index_evict)->page_num = page_num;
		(tlb_cache + index_evict)->frame_num = frame_num;
		updateTLBTimestamp(tlb_cache);
		(tlb_cache + index_evict)->timestamp = 1;
	}else{
		next = getNextTLBIndex(tlb_cache);
		(tlb_cache + next)->page_num = page_num;
		(tlb_cache + next)->frame_num = frame_num;
		(tlb_cache + next)->valid = 1;
		updateTLBTimestamp(tlb_cache);
		(tlb_cache + next)->timestamp = 1;
	}
}

void updateTLBTimestamp(tlb *tlb_cache){
	int i;
	for(i=0; i<TLB_ENTRIES; i++){
		if((tlb_cache + i)->valid == 0){
			return;
		}
		else{
			(tlb_cache + i)->timestamp++;
		}
	}
}

int tlbFull(tlb *tlb_cache){
	return ((tlb_cache + (TLB_ENTRIES - 1))->valid == 1) ? 1 : 0;
}

int getEvictIndex(tlb *tlb_cache){
	int i;
	int evict = 0;
	int cur_timestamp = tlb_cache->timestamp;
	for(i=1; i<TLB_ENTRIES; i++){
		if((tlb_cache + i)->timestamp > cur_timestamp){
			evict = i;
			cur_timestamp = (tlb_cache + i)->timestamp;
		}
	}
	return evict;
}

int getNextTLBIndex(tlb *tlb_cache){
	int i;
	for(i=0; i<TLB_ENTRIES; i++){
		if((tlb_cache + i)->valid == 0) return i;
	}
	return -1;
}

void updatePageTable(page_table *ptable, int frame_num){
	ptable->frame_num = frame_num;
	ptable->valid = 1;
}

void updateFrameToReplace(phys_mem *pmem){
	pmem->next_frame_to_replace++;
}