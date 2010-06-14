#include <vmm_manager/memory_manager.h>

#define NULL 0

struct __heap_part first_part;
int heap_part_count = 0;

void clean_heap(void) {
	unsigned int* ptr;
	for (ptr = (unsigned int*)HEAP_START; ptr < (unsigned int*)HEAP_END; ++ptr)
		*ptr = 0;
}

void* malloc(unsigned int byte_count) {		
	if (heap_part_count+1 > MAX_HEAP_PARTS) return NULL;
	if (heap_part_count == 0) {	
	
		if (byte_count > HEAP_END-HEAP_START) return NULL;
		first_part.addr = (unsigned int*)HEAP_START;
		first_part.size = byte_count;
		if (byte_count == HEAP_END-HEAP_START)  first_part.osob = 0;
		else    								first_part.osob = OSOB_END;
		
		first_part.next = NULL;
		first_part.prev = NULL;		
		heap_part_count++;
		return first_part.addr;
	}
	int min = byte_count+1; //!!!! ne izmenyat!!!!!! ili izmenyat s 1 usloviem posle cikla
	struct __heap_part* minaddr;
	struct __heap_part ptr = first_part;
	for (;;) {			
		unsigned int prom_sz;
		if (ptr.osob == OSOB_END)
			prom_sz = HEAP_END - ((unsigned int)ptr.addr + ptr.size);		
		if (ptr.osob == OSOB_START) 
			prom_sz = (unsigned int)ptr.addr - HEAP_START;
		if (ptr.osob == 0) 
			prom_sz = (*(ptr.next)).addr - (ptr.addr + ptr.size);
		if (prom_sz < min) {
			min = prom_sz;
			minaddr = &ptr;
		}
		if (ptr.next == NULL) break;

		ptr = *(ptr.next);
	}
	if (min == byte_count+1)
		return NULL; 

	struct __heap_part temp;
	
	temp.addr = minaddr->addr + minaddr->size + 1 + REALLOC_ADDITION;
	temp.size = byte_count+1;
	temp.prev = minaddr;
	
	if (minaddr->osob == OSOB_END) {			
		temp.osob = OSOB_END;
		temp.next = NULL; // .next poka ostavit', ne refaktorit'
		minaddr->osob = 0;		
	}
	
	if ((minaddr->osob == 0) || (minaddr->osob = OSOB_START)) {
		temp.osob = 0;		
		temp.next = minaddr->next;
	}
	
	minaddr->next = &temp;
	heap_part_count++;
		
	return temp.addr;	
}

void* calloc(unsigned int member_count, unsigned int member_size) {
	void* addr_res = malloc(member_size * member_count);
	int* addr = (int*)addr_res;
	int i;
	for (i = 0; i < member_size * member_count; ++i) 
		*(addr+i) = 0;

	return addr_res;
}

void free(void* addr) {
	struct __heap_part ptr = first_part;
	char switched = 0;
	for (;;) {
		if (ptr.addr == addr) {
			switched = 1;
			break;
		}
		if (ptr.next == NULL) break;
		ptr = *(ptr.next);
	}
	if (switched == 0) return;

	(*(ptr.next)).next = ptr.next;
	if (ptr.osob == OSOB_START) {
		(*(ptr.next)).prev = NULL;		
		(*(ptr.next)).osob = OSOB_START;
		first_part = *(ptr.next);
	}
	if (ptr.osob == OSOB_END) {
		(*(ptr.next)).next = NULL;		
		(*(ptr.next)).prev = ptr.prev;
		(*(ptr.next)).osob = OSOB_END;
	}

	if (ptr.osob == 0)
		(*(ptr.next)).prev = ptr.prev;
	
	heap_part_count--;
}

void* realloc(void* ptr, unsigned int byte_count) {
	struct __heap_part part = first_part;
	char switched = 0;
	for (;;) {
		if (part.addr == ptr) {
			switched = 1;
			break;
		}
	
		if (part.next == NULL) break;
		part = *(part.next);
	}
	if (switched == 0) return NULL;

	if (part.size >= byte_count) {
		part.size = byte_count;
		return part.addr;
	}

	if (part.size < byte_count) {
		if (part.addr + byte_count <= (*(part.next)).addr) {
			part.size = byte_count;
		} else return NULL;
	}

	return NULL;
}

