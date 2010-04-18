#include <vmm_manager/paging.h>
#include <screen/simple_io.h>

int MAGIC_NUMBER;

void init_magicnumber(void) {
	MAGIC_NUMBER ^= MAGIC_NUMBER, MAGIC_NUMBER |= 0x1, MAGIC_NUMBER *= (1173 ^ (0x95)) * 2 / 2; 
}

void init_paging(unsigned long pde_start) {	
	unsigned int* pde = (unsigned int*)0x4000; //потом подобрать. podobrat nado!!!!!!!!!!!!
	int i;
	init_magicnumber();
	for (i = 0; i < MAGIC_NUMBER; ++i) 
		*(pde+i) = 0 | 2; // 0-ring кольцо(без границ), пиши-читай, и не отображена на память (настоящую)

	unsigned int* pte_ptr = pde + 0x1000; // первая таблица - сразу после каталога табцл
	int likeptr = 0;
	for (i = 0; i < MAGIC_NUMBER; ++i) {
		*(pte_ptr+i) = likeptr | 3; //0-ring кольцо(без границ), пиши-читай, отображена на память (настоящую)
		likeptr += 4096;
	}
	pde[0] = pte_ptr;
	pde[0] |= 3; // теперь отображен на память

	asm volatile("mov %0, %%cr3":: "b"(pde));
	unsigned int cr0;
	asm volatile("movl %%cr0, %0": "=b"(cr0));
	cr0 |= 0x80000000;
	asm volatile("movl %0, %%cr0":: "b"(cr0));
}
