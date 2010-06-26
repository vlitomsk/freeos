#include <vmm_manager/memory_manager.h>
#include <screen/screen.h>

#define NULL 0

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long long u64;

struct __heap_part first_part;
int heap_part_count = 0;

void clean_heap(void) {
	unsigned char* ptr;
	for (ptr = (unsigned char*)HEAP_START; ptr < (unsigned char*)HEAP_END; ++ptr)
		*ptr = 0;
}

void reset_heap(void) {
	heap_part_count = 0;
}

#define MIN_DEF HEAP_END-HEAP_START+1

/*
struct __heap_part {
	void* addr;
	unsigned int size;
	unsigned char osob;
	struct __heap_part* next;
	struct __heap_part* prev;
};
*/

void malloc_print_dbg(void) {
	put_str("Part count: ");
	put_int(heap_part_count);
	put_char('\n');
	int i;
	struct __heap_part* ptr = &first_part;
	for (i = 0; i < heap_part_count; ++i) {
		put_str("For part "); put_int(i); put_str(": \n");
                put_str("  part address = "); put_int((int)ptr); put_char('\n');
		put_str("  size = "); put_int(ptr->size); put_char('\n');
		put_str("  osob = "); put_int(ptr->osob); put_char('\n');
		put_str("  next = "); put_int((u32)ptr->next); put_char('\n');
		put_str("  prev = "); put_int((u32)ptr->prev); put_char('\n');
		put_str("- - - - - - -\n");
		ptr = ptr->next;
	}
}

void* malloc(unsigned int byte_count) {
    if (heap_part_count + 1 > MAX_HEAP_PARTS) return NULL; // не хватает частей. в хидере объявлено
    if (heap_part_count == 0) { // первый раз в  первый класс!
        if (byte_count > HEAP_END - HEAP_START) {return NULL; put_str("nil:55\n\0");}  // много хватили

        first_part.addr = (void*)HEAP_START;
	first_part.size = byte_count;

//        if (byte_count == HEAP_END - HEAP_START)
 //           first_part.osob = 0;
   //     else
            first_part.osob = OSOB_START;

        first_part.next = NULL;
        first_part.prev = NULL;
        heap_part_count++;
        
        return first_part.addr;
    }
    
    // теперь найдем минимальный свободный кусок,с учетом прослойки для реаллок.
    int min = MIN_DEF; // сам размер минимального промежутка
    struct __heap_part* minaddr; // указатель на структуру ,после которой этот промежуток есть.
    struct __heap_part ptr = first_part; // "указатель" на текущую структуру
    unsigned char flag = 0;
    for (;;) {      
        unsigned int prom_sz, prom_sz2; // promsz - размер текущего промежутка, 2 - если блок еще OSOB_START
        // prom_sz - промежуток в сторону HEAP_END
        prom_sz2 = MIN_DEF;
        
        if (ptr.next == NULL) // блок последний
            prom_sz = HEAP_END - ((u64)ptr.addr + ptr.size);
        
        if (ptr.osob == OSOB_START) {// блок первый            
            prom_sz = (((ptr.next)->addr)) - ((u64)ptr.addr + ptr.size);            
            if (ptr.next == NULL) {
                prom_sz = HEAP_END - ((u64)ptr.addr + ptr.size);             
            }            
            prom_sz2 = (u64)ptr.addr - HEAP_START;
            flag = 1;
        }
        
        if (ptr.osob == 0)
            prom_sz = (*(ptr.next)).addr - (ptr.addr + ptr.size);

        if ((prom_sz < min) && (prom_sz + REALLOC_ADDITION >= byte_count)) {
            flag = 0;
            min = prom_sz;
            minaddr = &ptr;
        }

        if ((prom_sz2 < min) && (prom_sz2 + REALLOC_ADDITION >= byte_count) && (prom_sz2 > 0)) {
            flag = 1;
            min = prom_sz2;
            minaddr = &ptr;
        }
      
        if (ptr.next == NULL) break;

        ptr = *(ptr.next);

    }

    
    if (min == MIN_DEF) {
        //put_str("nil:115\0\n");
        return NULL;
    }

    struct __heap_part temp;    

    if (flag == 0) {
        temp.addr = minaddr->addr + minaddr->size + REALLOC_ADDITION + 1;
        temp.next = minaddr->next;
        minaddr->next = &temp;
        temp.prev = minaddr;
        temp.size = byte_count;
        temp.osob = 0;
        put_int((int)minaddr->next);

        heap_part_count++;
    } else if (flag == 1) {
        temp.addr = HEAP_START;
        temp.next = minaddr;
        minaddr->prev = &temp;
        temp.prev = NULL;
        temp.size = byte_count;
        temp.osob= OSOB_START;

        minaddr->osob = 0;

        heap_part_count++;
    } else return NULL;

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

//typedef unsigned char u8;

void * memset ( void * ptr, int value, int num ) {
    u8* i;
    for (i = (u8*)ptr; i < (u8*)ptr + num; ++i)
        *i = value;

    return ptr;
}

void *memcpy(void *dest, const void *src, int count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

int memcmp ( const void * ptr1, const void * ptr2, int num ) {
    int i;
    for (i = 0; i < num; ++i) {
        if ((*((u8*)((u8*)ptr1 + i))) !=  (*((u8*)((u8*)ptr2 + i)))) {
            if ((*((u8*)((u8*)ptr1 + i))) <  (*((u8*)((u8*)ptr2 + i)))) return -1;
            return 1;
        }
    }

    return 0;
}

/*
� ��� �� ���, ��� ������ �������,
��� ��� ����, ���� � �������� ��� ������.
��� ������ ��� ���������� � �����:
����� ����������� �����!
*/
