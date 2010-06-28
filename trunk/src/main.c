#include<main.h>

void kmain(){
	
  gdt_install();
  idt_install();
	isrs_install();
	irq_install();	
	
// раскоментить для проверки таймера	
//	timer_install();

  keyboard_install();
  
  __asm__ __volatile__ ("sti");
	
	init_video();
	puts("hello world gdt idt isr irq timer keyboard  [OK]\n");
	
// раскоментить для проверки на рабочесть прерываний	
//	int i;
//	i= 10/0;
//	puts(i);
}
